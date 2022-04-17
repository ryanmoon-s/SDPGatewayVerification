#ifndef ERPC_SERVER_H
#define ERPC_SERVER_H

#include <map>
#include "erpc_def.h"
#include "epoll_dispatcher.h"

#include "comm/ssltools/ssl_tool.h"
#include "comm/commdef/comm_def.h"
#include "comm/tlog/tlog.h"

class Server {
public:
    int Run();

public:
    Server(int port);
    ~Server();

private:
    std::map<std::string, int> ip_white_table_;
    EpollDispatcher* epoll_dispatcher_;
    int port_;
    int local_fd_ = 0;
};



#endif