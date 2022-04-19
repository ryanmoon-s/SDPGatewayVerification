#include "erpc_server.h"
#include "comm/ssltools/ssl_def.h"

int ErpcServer::Run()
{
    int ret = 0;

    TLOG_MSG(("Listen tcp_port:%d , udp_port:%d, ErpcServer begin to run ... ...", tcp_port_, udp_port_));
    while (true) 
    {
        ret = epoll_dispatcher_->Dispatch();
        iAssert(ret, ("Dispatch faild"));
    }

    return 0;
}

ErpcServer::ErpcServer(int tcp_port, int udp_port): tcp_port_(tcp_port), udp_port_(udp_port) 
{
    epoll_dispatcher_ = new EpollDispatcher(tcp_port, udp_port);
}

ErpcServer::~ErpcServer()
{
    delete epoll_dispatcher_;
}

void ErpcServer::RegisterService(ErpcService* service)
{
    ErpcConfig* config = ErpcConfig::GetInstance();
    config->SetServiceObj(service);
}
