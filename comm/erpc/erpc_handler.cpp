#include "erpc_handler.h"

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
    iAssert(ret, ("_HandleRead"));

    ret = _ParseRequestAndForward(PacketReq, PacketRsp);
    iAssert(ret, ("_ParseRequestAndForward"));

    ret = _HandleWrite(PacketRsp, connector);
    iAssert(ret, ("_HandleWrite"));

    return 0;
}

int  ErpcHandler::HandleNetAccept(int listen_fd, FdDataType& fd_data)
{
    fd_data.connector = std::make_shared<SSLConnector>(SSL_CRT_SERVER, SSL_KEY_SERVER);;

    int tmp_fd = accept(listen_fd, nullptr, 0);
    int ret = fd_data.connector->SSLAccept(tmp_fd);
    iAssert(ret, ("SSLAccept"));

    fd_data.fd = tmp_fd;
    fd_data.event_type = EPOLLIN;
    fcntl(tmp_fd, F_SETFL, fcntl(tmp_fd, F_GETFL, 0) | O_NONBLOCK);

    return 0;
}

int ErpcHandler::ClientRequest(const Packet& PacketReq, Packet& PacketRsp, std::shared_ptr<SSLConnector> connector, const std::string& ip, int port)
{
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

    std::string PacketReqStr;
    std::string PacketRspStr;
    ret = _PackDataToString(PacketReqStr, PacketReq);
    ret = _HandleWrite(PacketReqStr, connector);
    if (ret != PacketReqStr.size()) 
    {
        TLOG_ERR(("Write faild, write:%d/%d", ret, PacketReqStr.size()));
    }

    // wait for response
    ret = _HandleRead(PacketRspStr, connector);
    iAssert(ret, ("Wait for response faild"));

    return 0;
}

int ErpcHandler::_ParseRequestAndForward(const std::string& PacketReqStr, std::string& PacketRspStr)
{
    int ret = 0;
    Packet packet;
    std::string response;

    ret = _ParseDataFromString(packet, PacketReqStr);
    iAssert(ret, ("_ParseDataFromString"));

    ret = _RequestForwardWithCmd(packet.cmdid, packet.body, response, packet.header);
    if (ret < 0) {
        packet.header.ret_code = ret;
        packet.header.ret_msg = "An unknown error occurred on the server";
        return ret;
    }

    packet.body = response;
    _PackDataToString(PacketRspStr, packet);
    iAssert(ret, ("_PackDataToString"));

    return ret;
}

int ErpcHandler::_RequestForwardWithCmd(int32_t cmdid, const std::string& request, std::string& response, Header& header)
{
    int ret = 0;

    if (cmdid == CMD_TEST_FUNC) {
        RPC_CALL_FORWARD(TestFunc, request, response, header);
        return 0;
    }

    return erpc_def::kErrServiceNotFound;
}

int ErpcHandler::_ParseDataFromString(Packet& Packet, const std::string& str)
{
    /*
        组包格式
        4     struct  protobuf
        cmdid header  body
    */

    int min_size = sizeof(uint32_t) + sizeof(Header);
    if (str.size() < min_size)
    {
        return kPacketLengthErr;
    }

    // cmdid
    std::string tmpstr = str.substr(0, sizeof(uint32_t));
    Packet.cmdid = MemoryCorpToObject<uint32_t>(tmpstr);

    // header
    tmpstr = str.substr(sizeof(uint32_t), sizeof(Header));
    Packet.header = MemoryCorpToObject<Header>(tmpstr);
    
    // body
    Packet.body = str.substr(sizeof(uint32_t) + sizeof(Header));

    return 0;
}

int ErpcHandler::_PackDataToString(std::string& str, const Packet& Packet)
{
    /*
        组包格式
        4     struct  protobuf
        cmdid header  body
    */
    
    str.clear();

    // cmdid
    str.append(MemoryCopyToString(Packet.cmdid));

    // header
    str.append(MemoryCopyToString(Packet.header));

    // body
    str.append(Packet.body);

    return 0;
}

int ErpcHandler::_HandleRead(std::string& outstr, std::shared_ptr<SSLConnector> connector)
{
    char buff[RD_BUF_SIZE];
    int n = 0, total = 0;
    while (true) 
    {
        n = connector->SSLRead(buff, RD_BUF_SIZE);
        if (n == 0)
        {
            break;
        }
        else if (n < 0)
        {
            if (errno == EAGAIN)
            {
                break;
            }
            else if (errno == EINTR || errno == EWOULDBLOCK)
            {
                continue;
            }
            return -1;
        }
        total += n;
    }

    outstr = std::string(buff, total);
    return total;
}

int ErpcHandler::_HandleWrite(const std::string& instr, std::shared_ptr<SSLConnector> connector)
{
    int n = 0, total = 0;
    while (true)
    {
        n = connector->SSLWrite(instr.c_str(), instr.size());
        if (n == 0)
        {
            break;
        }
        else if (n < 0)
        {
            if (errno == EAGAIN)
            {
                break;
            }
            else if (errno == EINTR || errno == EWOULDBLOCK)
            {
                continue;
            }
            return -1;
        }
        total += n;
    }

    return total;
}