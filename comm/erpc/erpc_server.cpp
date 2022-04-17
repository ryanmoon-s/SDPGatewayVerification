#include "erpc_server.h"
#include "comm/ssltools/ssl_def.h"

int Server::Run()
{
    int ret = 0;

    TLOG_MSG(("Listen port:%d , Server begin to run ... ...", port_));
    while (true) 
    {
        ret = epoll_dispatcher_->Dispatch();
        iAssert(ret, ("Dispatch faild"));
    }

    return 0;
}

Server::Server(int port) {
    int ret = 0;

    port_ = port;
    epoll_dispatcher_ = new EpollDispatcher(port_);
}

Server::~Server()
{
    delete epoll_dispatcher_;
}
