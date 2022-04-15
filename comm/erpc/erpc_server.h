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
    Server();
    int Run(int port);

private:
    int _MakeListenFd(int port);

    EpollDispatcher epoll_dispatcher_;
    int listen_fd_;
};



#endif