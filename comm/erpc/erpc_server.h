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
    // 1、注册服务，传入子类指针
    void RegisterService(ErpcService* service);

    // 2、启动运行
    int Run();


public:
    ErpcServer(int tcp_port, int udp_port);
    ~ErpcServer();

private:
    std::map<std::string, int> ip_white_table_;
    EpollDispatcher* epoll_dispatcher_;
    int tcp_port_;
    int udp_port_;
    int local_fd_ = 0;
};
