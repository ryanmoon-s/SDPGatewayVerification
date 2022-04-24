#pragma once

#include <map>
#include "erpc_def.h"
#include "erpc_server.h"
#include "epoll_dispatcher.h"
#include "erpc_service.h"

#include "comm/ssltools/ssl_tool.h"
#include "comm/commdef/comm_def.h"
#include "comm/tlog/tlog.h"

// 服务端身份识别
enum SERVER_Identification {
    ID_CONTROLLER = 1,
    ID_GATEWAY = 2,
    ID_APPLICATION = 3,
};

class ErpcServer {
public:
    // 1、注册服务
    // 传入子类指针、证书、私钥
    void RegisterService(ErpcService* service, const std::string& cert, const std::string& key);

    // 2、启动运行
    int Run();


public:
    // controller gateway
    ErpcServer(int id, const std::string& ip, int tcp_port, int udp_port);
    // application
    ErpcServer(int id, const std::string& ip, int tcp_port);

    ~ErpcServer();

private:
    EpollDispatcher* epoll_dispatcher_;
    int id;  // SERVER_Identification

    std::string listen_ip_;

    int tcp_port_;
    int udp_port_;
};
