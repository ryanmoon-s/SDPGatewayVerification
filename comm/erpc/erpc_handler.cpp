#include "erpc_handler.h"
#include "comm/ssltools/ssl_def.h"

using namespace erpc_def;
using namespace erpc_service;

#define RPC_CALL_FORWARD(FUNC, request, response, header)               \
    do {                                                                \
        erpc::FUNC##Req req;                                            \
        erpc::FUNC##Rsp rsp;                                            \
        req.ParseFromString(request);                                   \
        ret = FUNC(req, rsp, header);                                   \
        rsp.SerializeToString(&response);                               \
    } while(0)
      
int ErpcHandler::HandleNetRquest(const FdDataType& fd_data)
{
    int fd = fd_data.fd;
    auto connector = fd_data.connector;
    
    std::string PacketReq;
    std::string PacketRsp;

    int ret = _HandleRead(PacketReq, connector);
    if (ret == 0)
    {
        TLOG_MSG(("_HandleRead 0 bytes, can't handle"));
        return 0;
    }
    else if (ret < 0)
    {
        TLOG_ERR(("_HandleRead error"));
        return ret;
    }

    ret = _ParseRequestAndForward(PacketReq, PacketRsp);
    iAssert(ret, ("_ParseRequestAndForward"));

    ret = _HandleWrite(PacketRsp, connector);
    iAssert(ret, ("_HandleWrite"));

    return 0;
}

int  ErpcHandler::HandleNetAccept(int listen_fd, FdDataType& fd_data)
{
    fd_data.connector = std::make_shared<SSLConnector>(SSL_CRT_SERVER, SSL_KEY_SERVER, 1);;

    int tmp_fd = accept(listen_fd, nullptr, 0);
    iAssert(tmp_fd, ("accept listen_fd:%d, errno:%d, errmsg:%s", listen_fd, errno, strerror(errno)));
    
    int ret = fd_data.connector->SSLAccept(tmp_fd);
    iAssert(ret, ("SSLAccept"));
    
    fd_data.fd = tmp_fd;
    fd_data.event_type = EPOLLIN | EPOLLET;

    // 非阻塞
    fcntl(tmp_fd, F_SETFL, fcntl(tmp_fd, F_GETFL, 0) | O_NONBLOCK);
    
    TLOG_DBG(("HandleNetAccept fd:%d", tmp_fd));
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
    ret = _PackDataToString(PacketReqStr, PacketReq, 0);

    ret = _HandleWrite(PacketReqStr, connector);
    if (ret != PacketReqStr.size()) 
    {
        TLOG_ERR(("Write faild, write:%d/%d", ret, PacketReqStr.size()));
    }

    // wait for response
    ret = _HandleRead(PacketRspStr, connector);
    iAssert(ret, ("Wait for response faild"));

    _ParseDataFromString(PacketRsp, PacketRspStr, 1);

    return 0;
}

int ErpcHandler::_ParseRequestAndForward(const std::string& PacketReqStr, std::string& PacketRspStr)
{
    int ret = 0;
    Packet packet;
    std::string response;
 
    ret = _ParseDataFromString(packet, PacketReqStr, 0);
    iAssert(ret, ("_ParseDataFromString"));
    
    ret = _RequestForwardWithCmd(packet.cmdid, packet.body, response, packet.header);
    if (ret < 0) {
        packet.header.ret_code = ret;
        packet.header.ret_msg = "An unknown error occurred on the server";
        return ret;
    }
    
    packet.body = response;
    _PackDataToString(PacketRspStr, packet, 1);
    iAssert(ret, ("_PackDataToString"));
    
    return ret;
}

int ErpcHandler::_RequestForwardWithCmd(int32_t cmdid, const std::string& request, std::string& response, Header& header)
{
    int ret = 0;

    if (cmdid == CMD_FUNC_REVERSE) {
        RPC_CALL_FORWARD(FuncReverse, request, response, header);
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
    TLOG_DBG(("_ParseDataFromString Begin size:%d", str.size()));

    int min_size = 4 + (is_with_header ? 4 + HEADER_RET_MSG_LIMIT_SIZE : 0);
    if (str.size() < min_size)
    {
        return kPacketLengthErr;
    }
    
    // cmdid
    std::string tmpstr = str.substr(0, 4);
    Packet.cmdid = MemoryCorpToObject<uint32_t>(tmpstr);
    TLOG_DBG(("Parse cmdid:%u", Packet.cmdid));

    // header
    if (is_with_header)
    {
        tmpstr = str.substr(4, 4);
        Packet.header.ret_code = MemoryCorpToObject<int>(tmpstr);
        Packet.header.ret_msg = str.substr(8, HEADER_RET_MSG_LIMIT_SIZE);
        TLOG_DBG(("Parse header ret_code:%d, ret_msg:%s", Packet.header.ret_code, Packet.header.ret_msg.c_str()));
    }
    
    // body
    Packet.body = str.substr(min_size);
    TLOG_DBG(("Parse size:%d", Packet.body.size()));

    return 0;
}

int ErpcHandler::_PackDataToString(std::string& str, const Packet& Packet, int is_with_header)
{
    /*
        组包格式
        cmdid header body
        4     4+50   left
    */
    
    str.clear();

    // cmdid
    str.append(MemoryCopyToString(Packet.cmdid));

    // header
    if (is_with_header)
    {
        str.append(MemoryCopyToString(Packet.header.ret_code));

        int msg_size = Packet.header.ret_msg.size();
        str.append(Packet.header.ret_msg.substr(0, HEADER_RET_MSG_LIMIT_SIZE));
        TLOG_DBG(("str:%s, %d", Packet.header.ret_msg.c_str(), HEADER_RET_MSG_LIMIT_SIZE - msg_size));
        // 填充至 HEADER_RET_MSG_LIMIT_SIZE
        str.append(std::max(0, HEADER_RET_MSG_LIMIT_SIZE - msg_size), '\0');
    }

    // body
    str.append(Packet.body);

    TLOG_DBG(("_PackDataToString End size:%d", str.size()));

    return 0;
}


int ErpcHandler::_HandleRead(std::string& outstr, std::shared_ptr<SSLConnector> connector)
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
    TLOG_DBG(("_HandleRead size:%d", outstr.size()));
    return n;
}

int ErpcHandler::_HandleWrite(const std::string& instr, std::shared_ptr<SSLConnector> connector)
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

    TLOG_DBG(("_HandleWrite size:%d", total));
    return total;
}