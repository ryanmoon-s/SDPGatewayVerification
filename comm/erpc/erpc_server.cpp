#include "erpc_server.h"
#include "comm/ssltools/ssl_def.h"

int ErpcServer::Run()
{
    int ret = 0;

    if (id_ == erpc::ID_CONTROLLER)
    {
        TLOG_MSG(("Listen ip:%s, tcp_port:%d , udp_port:%d", listen_ip_.c_str(), tcp_port_, udp_port_));
        TLOG_MSG(("CONTROLLER begin to run ... ..."));
    }
    else if (id_ == erpc::ID_GATEWAY)
    {
        TLOG_MSG(("Listen ip:%s, tcp_port:%d , udp_port:%d", listen_ip_.c_str(), tcp_port_, udp_port_));
        TLOG_MSG(("GATEWAY begin to run ... ..."));
    }
    else if (id_ == erpc::ID_APPLICATION)
    {
        TLOG_MSG(("Listen ip:%s, tcp_port:%d", listen_ip_.c_str(), tcp_port_));
        TLOG_MSG(("APPLICATION begin to run ... ..."));
    }

    while (true) 
    {
        ret = epoll_dispatcher_->Dispatch();
        iAssert(ret, ("Dispatch faild"));
    }

    return 0;
}

// controller gateway
ErpcServer::ErpcServer(int id, const std::string& ip, int tcp_port, int udp_port)
    : id_(id), listen_ip_(ip), tcp_port_(tcp_port), udp_port_(udp_port) 
{
    epoll_dispatcher_ = new EpollDispatcher(id, ip, tcp_port, udp_port);
}

// application
ErpcServer::ErpcServer(int id, const std::string& ip, int tcp_port)
    : id_(id), listen_ip_(ip), tcp_port_(tcp_port)
{
    epoll_dispatcher_ = new EpollDispatcher(id, ip, tcp_port);
}

ErpcServer::~ErpcServer()
{
    delete epoll_dispatcher_;
}

void ErpcServer::RegisterService(ErpcService* service, const std::string& cert, const std::string& key)
{
    ErpcConfig* config = ErpcConfig::GetInstance();
    config->SetServiceObj(service);
    config->SetServerCert(cert, key);
}
