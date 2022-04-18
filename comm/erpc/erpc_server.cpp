#include "erpc_server.h"
#include "comm/ssltools/ssl_def.h"

int Server::Run()
{
    int ret = 0;

    TLOG_MSG(("Listen tcp_port:%d , udp_port:%d, Server begin to run ... ...", tcp_port_, udp_port_));
    while (true) 
    {
        ret = epoll_dispatcher_->Dispatch();
        iAssert(ret, ("Dispatch faild"));
    }

    return 0;
}

Server::Server(int tcp_port, int udp_port): tcp_port_(tcp_port), udp_port_(udp_port) {
    int ret = 0;

    epoll_dispatcher_ = new EpollDispatcher(tcp_port, udp_port);
}

Server::~Server()
{
    delete epoll_dispatcher_;
}
