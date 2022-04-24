#pragma once

#include <map>
#include "erpc_def.h"
#include "erpc_server.h"
#include "epoll_dispatcher.h"
#include "erpc_service.h"

#include "comm/ssltools/ssl_tool.h"
#include "comm/commdef/comm_def.h"
#include "comm/tlog/tlog.h"

class ErpcServer {
public:
    // 1、注册服务
    // 传入子类指针、证书、私钥
    void RegisterService(ErpcService* service, const std::string& cert, const std::string& key);

    // 2、启动运行
    int Run();


public:
    ErpcServer(const std::string& ip, int tcp_port, int udp_port);
    ~ErpcServer();

private:
    EpollDispatcher* epoll_dispatcher_;

    std::string listen_ip_;

    int tcp_port_;
    int udp_port_;

    int local_fd_ = 0;
};
