#ifndef ERPC_HANDLER_H
#define ERPC_HANDLER_H

#include "epoll_dispatcher.h"
#include "erpc_service.h"
#include "erpc_config.h"
#include "erpc_def.h"
#include "comm/proto/erpc.pb.h"

using namespace erpc_def;
struct FdDataType;

// 5M
#define RD_BUF_SIZE 1024*1024*5 

class ErpcHandler {
public:
    // 服务器端入口
    int HandleNetRquest(const FdDataType& fd_data);
    int HandleNetAccept(int listen_fd, FdDataType& data);

    // 客户端发送请求
    int ClientRequest(const Packet& PacketReq, Packet& PacketRsp, std::shared_ptr<SSLConnector> connector, const std::string& ip, int port);

    // 操作IP白名单
    void IpWhiteTableOp(int op, std::string ip);

public:
    // 读取与发送数据包
    int SSLRead(std::string& outstr, std::shared_ptr<SSLConnector> connector);
    int SSLWrite(const std::string& instr, std::shared_ptr<SSLConnector> connector);
    int SimpleRead(std::string& outstr, int fd);
    int SimpleWrite(const std::string& instr, int fd);

public:
    ErpcHandler() {}
    ErpcHandler(const ErpcHandler&) {}

private:
    // 解包网络请求
    int _ParseRequestAndForward(const std::string& PacketReqStr, std::string& PacketRspStr);
    // 转发网络请求至具体服务
    int _RequestForwardWithCmd(int32_t cmdid, const std::string& request, std::string& response);

    // Packet 解包与打包
    // is_with_header: 是否处理Header 0 or 1
    int _ParseDataFromString(Packet& packet, const std::string& str, int is_with_header);
    void _PackDataToString(std::string& str, const Packet& packet, int is_with_header);

    // 将二进制内容拷贝进string
    template<typename T>
    std::string _MemoryCopyToString(const T& data);
    // 将二进制内容从string中取出
    template<typename T>
    T _MemoryCorpToObject(const std::string& str);
};



#endif