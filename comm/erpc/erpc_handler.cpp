#include "erpc_handler.h"
#include "comm/ssltools/ssl_def.h"

using namespace erpc_def;

#define RPC_CALL_FORWARD(FUNC, request, response)                           \
    do {                                                                    \
        erpc::FUNC##Req req;                                                \
        erpc::FUNC##Rsp rsp;                                                \
        req.ParseFromString(request);                                       \
        ErpcService* service = ErpcConfig::GetInstance()->GetServiceObj();  \
        ret = service->FUNC(req, rsp);                                      \
        rsp.SerializeToString(&response);                                   \
    } while(0)

int ErpcHandler::HandleTCPRequest(const FdDataType& fd_data)
{
    int fd = fd_data.fd;
    auto connector = fd_data.connector;
    
    std::string PacketReq;
    std::string PacketRsp;

    int ret = HandleRead(PacketReq, connector);
    if (ret == 0)
    {
        TLOG_MSG(("HandleRead 0 bytes, can't handle"));
        return 0;
    }
    else if (ret < 0)
    {
        TLOG_ERR(("HandleRead error"));
        return ret;
    }

    ret = _ParseRequestAndForward(PacketReq, PacketRsp);
    iAssert(ret, ("_ParseRequestAndForward"));

    ret = HandleWrite(PacketRsp, connector);
    iAssert(ret, ("HandleWrite"));

    return 0;
}

int ErpcHandler::HandleTCPAccept(int listen_fd, FdDataType& fd_data)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    int tmp_fd = accept(listen_fd, (struct sockaddr*)&addr, &len);
    iAssert(tmp_fd, ("accept listen_fd:%d, errno:%d, errmsg:%s", listen_fd, errno, strerror(errno)));
    
    //  == reject illegal ip
    std::string ip(inet_ntoa(addr.sin_addr));
    ErpcConfig* config = ErpcConfig::GetInstance();

    config->IpWhiteTableOp(IP_WHITE_TABLE_ADD, ip);

    bool result = config->IsIpInWhiteTable(ip);
    if (!result)
    {
        TLOG_MSG(("HandleTCPAccept reject ip:%s", ip.c_str()));
        close(tmp_fd);
        return kIpNotInWhiteTable;
    }

    fd_data.fd = tmp_fd;
    fd_data.event_type = EPOLLIN | EPOLLET;
    fd_data.connector = std::make_shared<SSLConnector>(SSL_CRT_SERVER, SSL_KEY_SERVER, 1);;

    int ret = fd_data.connector->SSLAccept(tmp_fd);
    iAssert(ret, ("SSLAccept"));
    
    // 非阻塞
    fcntl(tmp_fd, F_SETFL, fcntl(tmp_fd, F_GETFL, 0) | O_NONBLOCK);
    
    TLOG_MSG(("accept fd:%d, ip:%s, port:%u", tmp_fd, inet_ntoa(addr.sin_addr), addr.sin_port));
    return 0;
}

int ErpcHandler::HandleUDPRequest(const FdDataType& fd_data)
{
    int ret = 0, from_port = 0;
    int fd = fd_data.fd;
    spa::SPAPacket spaPacket;
    spa::SPAVoucher spaVoucher;
    std::string msg, from_ip;
    
    ret = UDPRecv(msg, fd, from_ip, from_port);
    iAssert(ret, ("UDPRecv faild"));

    spaPacket.ParseFromString(msg);
    ret = SPATools().DecryptVoucher(spaVoucher, spaPacket);
    iAssert(ret, ("DecryptVoucher faild"));

    ErpcService* service = ErpcConfig::GetInstance()->GetServiceObj();
    ret = service->FuncUdpRecv(spaVoucher);
    iAssert(ret, ("FuncUdpRecv error ret:%d", ret));

    TLOG_MSG(("HandleUDPRequest success, from ip:%s, port:%d", from_ip.c_str(), from_port));
    return 0;
}

int ErpcHandler::ClientRequest(const Packet& PacketReq, Packet& PacketRsp, std::shared_ptr<SSLConnector> connector, const std::string& ip, int port)
{
    // Generate socket
    int fd = 0, ret = 0;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);  
    addr.sin_addr.s_addr = inet_addr(ip.c_str()); 

    ret = connect(fd, (struct sockaddr*)&addr, (socklen_t)sizeof(addr));
    iAssert(ret, ("connect"));

    ret = connector->SSLConnect(fd);
    iAssert(ret, ("SSLConnect"));

    // Request
    std::string PacketReqStr;
    std::string PacketRspStr;
    _PackDataToString(PacketReqStr, PacketReq, 0);

    ret = HandleWrite(PacketReqStr, connector);
    if (ret != PacketReqStr.size()) 
    {
        TLOG_ERR(("Write faild, write:%d/%d", ret, PacketReqStr.size()));
    }

    ret = HandleRead(PacketRspStr, connector);
    iAssert(ret, ("Wait for response faild"));

    ret = _ParseDataFromString(PacketRsp, PacketRspStr, 1);
    iAssert(ret, ("Parse faild"));

    return 0;
}

int ErpcHandler::_ParseRequestAndForward(const std::string& PacketReqStr, std::string& PacketRspStr)
{
    int ret = 0;
    Packet packet;
    std::string response;
 
    ret = _ParseDataFromString(packet, PacketReqStr, 0);
    iAssert(ret, ("Parse faild"));
    
    ret = _RequestForwardWithCmd(packet.cmdid, packet.body, response);
    packet.header.ret_code = ret;
    if (ret == 0) {
        packet.header.ret_msg = "Request OK";
    } 
    else if (ret < 0)
    {
        packet.header.ret_msg = "An unknown error occurred on the server";
        return ret;
    }
    
    packet.body = response;
    _PackDataToString(PacketRspStr, packet, 1);
    
    return ret;
}

int ErpcHandler::_RequestForwardWithCmd(int32_t cmdid, const std::string& request, std::string& response)
{
    int ret = 0;

    if (cmdid == ErpcService::CMD_FUNC_REVERSE) {
        TLOG_MSG(("RPC forward to cmdid:%d"));
        RPC_CALL_FORWARD(FuncReverse, request, response);
        return 0;
    }

    return erpc_def::kErrServiceNotFound;
}

int ErpcHandler::_ParseDataFromString(Packet& Packet, const std::string& str, int is_with_header)
{
    /*
        组包格式
        cmdid header body
        4     4+50   left
    */
    TLOG_DBG(("Begin size:%d", str.size()));

    int min_size = 4 + (is_with_header ? 4 + HEADER_RET_MSG_LIMIT_SIZE : 0);
    if (str.size() < min_size)
    {
        return kPacketLengthErr;
    }
    
    // cmdid
    std::string tmpstr = str.substr(0, 4);
    Packet.cmdid = _MemoryCorpToObject<uint32_t>(tmpstr);
    TLOG_DBG(("Parse cmdid:%u", Packet.cmdid));

    // header
    if (is_with_header)
    {
        tmpstr = str.substr(4, 4);
        Packet.header.ret_code = _MemoryCorpToObject<int>(tmpstr);
        Packet.header.ret_msg = str.substr(8, HEADER_RET_MSG_LIMIT_SIZE);
        TLOG_DBG(("Parse header ret_code:%d, ret_msg:%s", Packet.header.ret_code, Packet.header.ret_msg.c_str()));
    }
    
    // body
    Packet.body = str.substr(min_size);
    TLOG_DBG(("Parse size:%d", Packet.body.size()));

    return 0;
}

void ErpcHandler::_PackDataToString(std::string& str, const Packet& Packet, int is_with_header)
{
    /*
        组包格式
        cmdid header body
        4     4+50   left
    */
    
    str.clear();

    // cmdid
    str.append(_MemoryCopyToString(Packet.cmdid));

    // header
    if (is_with_header)
    {
        str.append(_MemoryCopyToString(Packet.header.ret_code));

        int msg_size = Packet.header.ret_msg.size();
        str.append(Packet.header.ret_msg.substr(0, HEADER_RET_MSG_LIMIT_SIZE));
        // ret_msg 填充
        str.append(std::max(0, HEADER_RET_MSG_LIMIT_SIZE - msg_size), '\0');
    }

    // body
    str.append(Packet.body);

    TLOG_DBG(("End size:%d", str.size()));
}

template<typename T>
std::string ErpcHandler::_MemoryCopyToString(const T& data)
{
    char* p = (char*)&data;
    return std::string(p, sizeof(T));
}

template<typename T>
T ErpcHandler::_MemoryCorpToObject(const std::string& str)
{
    return *(T*)(str.c_str());
}

int ErpcHandler::HandleRead(std::string& outstr, std::shared_ptr<SSLConnector> connector)
{
    char buff[RD_BUF_SIZE];
    int n = 0, total = 0;

    n = connector->SSLRead(buff, RD_BUF_SIZE);
    if (n == 0)
    {
        // close socket TODO
    }
    else if (n < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return n;
        }
        else if (errno == EINTR)
        {
            return n;
        }
        return -1;
    }

    outstr = std::string(buff, n);
    TLOG_DBG(("size:%d", outstr.size()));
    return n;
}

int ErpcHandler::HandleWrite(const std::string& instr, std::shared_ptr<SSLConnector> connector)
{
    int n = 0, total = 0, w_size = instr.size();
    std::string str = instr;
    
    while (true)
    {
        str = str.substr(total);
        n = connector->SSLWrite(str.c_str(), str.size());
        if (n == 0)
        {
            break;
        }
        else if (n < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            else if (errno == EINTR)
            {
                continue;
            }
            return -1;
        }
        total += n;
        if (total == w_size)
        {
            break;
        }
    }

    TLOG_DBG(("size:%d", total));
    return total;
}

int ErpcHandler::HandleRead(std::string& outstr, int fd)
{
    char buff[RD_BUF_SIZE];
    int n = 0, total = 0;

    n = read(fd, buff, RD_BUF_SIZE);
    if (n == 0)
    {
        // close socket TODO
    }
    else if (n < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return n;
        }
        else if (errno == EINTR)
        {
            return n;
        }
        return -1;
    }

    outstr = std::string(buff, n);
    TLOG_DBG(("size:%d", outstr.size()));
    return n;
}

int ErpcHandler::HandleWrite(const std::string& instr, int fd)
{
    int n = 0, total = 0, w_size = instr.size();
    std::string str = instr;
    
    while (true)
    {
        str = str.substr(total);
        n = write(fd, str.c_str(), str.size());
        if (n == 0)
        {
            break;
        }
        else if (n < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            else if (errno == EINTR)
            {
                continue;
            }
            return -1;
        }
        total += n;
        if (total == w_size)
        {
            break;
        }
    }

    TLOG_DBG(("size:%d", total));
    return total;
}

int ErpcHandler::UDPRecv(std::string& outstr, int fd, std::string& from_ip, int& from_port)
{
    int ret = 0, n = 0;
    char buff[RD_BUF_SIZE];
    struct sockaddr_in addr;
    socklen_t len;

    len = sizeof(addr);
    n = recvfrom(fd, buff, RD_BUF_SIZE, 0, (sockaddr*)&addr, &len);
    if (ret == 0)
    {
        // re new socket ??
    }
    else if (ret < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return 0;
        }
        else if (errno == EINTR)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }

    outstr.assign(buff, n);
    from_ip.assign(inet_ntoa(addr.sin_addr));
    from_port = addr.sin_port;

    TLOG_DBG(("size:%d", outstr.size()));

    return 0;
}

int ErpcHandler::UDPSend(const std::string& outstr, const std::string& dest_ip, const int& dest_port)
{
    int ret = 0, fd = 0;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_DGRAM, 0); 
    iAssert(fd, ("socket faild"));
 
	addr.sin_family = AF_INET;
	addr.sin_port = htons(dest_port);
	addr.sin_addr.s_addr = inet_addr(dest_ip.c_str());

    ret = sendto(fd, outstr.c_str(), outstr.size(), 0, (sockaddr*)&addr, sizeof(addr));
    iAssert(ret, ("sendto faild, ip:%s, port:%d, errmsg:%s", dest_ip.c_str(), dest_port, strerror(errno)));

    return 0;
}
