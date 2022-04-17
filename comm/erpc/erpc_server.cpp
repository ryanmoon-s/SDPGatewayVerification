#include "erpc_server.h"
#include "comm/ssltools/ssl_def.h"

Server::Server() {

}

int Server::Run(int port)
{
    int ret = 0;

    ret = _MakeListenFd(port);
    iAssert(ret, ("_MakeListenFd"));

    // white table
    ip_white_table_["127.0.0.1"] = 1;

    TLOG_MSG(("Listen port:%d , Server begin to run ... ...", port));
    while (true) 
    {
        ret = epoll_dispatcher_.Dispatch(listen_fd_, ip_white_table_);
        iAssert(ret, ("Dispatch faild, listen_fd:%d", listen_fd_));
    }

    return 0;
}

int Server::_MakeListenFd(int port)
{
    int ret = 0, resue = 1;
    struct sockaddr_in addr;
    FdDataType fd_data;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(IP_CONTROLLER);
    addr.sin_port = htons(port);

    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0); 
    iAssert(listen_fd_, ("socket: listen_fd:%d", listen_fd_));

    ret = setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, (const void *)&resue, sizeof(int));
    iAssert(ret, ("setsockopt: listen_fd:%d", listen_fd_));

    ret = bind(listen_fd_, (struct sockaddr*)&addr, sizeof(addr));
    iAssert(ret, ("bind: listen_fd:%d", listen_fd_));

    ret = listen(listen_fd_, 1024);
    iAssert(ret, ("listen: listen_fd:%d", listen_fd_));

    fcntl(listen_fd_, F_SETFL, fcntl(listen_fd_, F_GETFL, 0) | O_NONBLOCK); 

    fd_data.fd = listen_fd_;
    fd_data.event_type = EPOLLIN | EPOLLET | EPOLLRDHUP;
    fd_data.connector = std::make_shared<SSLConnector>(SSL_CRT_SERVER, SSL_KEY_SERVER, 1);

    ret = epoll_dispatcher_.DispatcherAdd(fd_data);
    iAssert(ret, ("DispatcherAdd: listen_fd_"));

    return 0;
}