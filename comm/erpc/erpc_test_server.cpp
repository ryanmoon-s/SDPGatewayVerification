#include "erpc_server.h"
#include "comm/commdef/comm_def.h"
#include "comm/ssltools/ssl_def.h"

int mini_epoll_proc();
int mini_server_ssl();

int main()
{
    Server server;
    server.Run(TCP_PORT_CONTROLLER);
}

int mini_epoll_proc()
{ 
    int ret = 0, listen_fd_ = 0, epollfd = 0;
    struct sockaddr_in addr;
    FdDataType fd_data;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(6001);

    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0); 
    iAssert(listen_fd_, ("socket: listen_fd_"));

    ret = bind(listen_fd_, (struct sockaddr*)&addr, sizeof(addr));
    iAssert(ret, ("bind: listen_fd_"));

    ret = listen(listen_fd_, 1024);
    iAssert(ret, ("listen: listen_fd_"));

    epollfd = epoll_create1(0);
    iAssert(epollfd, ("epoll_create1"));

    struct epoll_event event;
    event.data.fd = listen_fd_;
    event.events = EPOLLIN;

    ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_fd_, &event);
    iAssert(ret, ("epoll_ctl add epollfd:%d fd:%d type:%u errno:%d errmsg:%s", epollfd, listen_fd_, event.events, errno, strerror(errno)));

    struct epoll_event *events;
    events = (struct epoll_event*)calloc(MAXEVENTS, sizeof(struct epoll_event));
    iAssertNull(events, ("calloc"));

    TLOG_DBG(("listen_fd:%d, epollfd:%d, type:%u", listen_fd_, epollfd, event.events));

    int nums = epoll_wait(epollfd, events, MAXEVENTS, -1);
    iAssert(nums, ("epoll_wait faild, epollfd:%d, errno:%d, errmsg:%s", epollfd, errno, strerror(errno)));
}

int mini_server_ssl()
{
    // listen fd
    int ret = 0, resue = 1, listen_fd_ = 0;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(IP_CONTROLLER);
    addr.sin_port = htons(TCP_PORT_CONTROLLER);

    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0); 
    iAssert(listen_fd_, ("socket: listen_fd:%d", listen_fd_));

    ret = setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, (const void *)&resue, sizeof(int));
    iAssert(ret, ("setsockopt: listen_fd:%d", listen_fd_));

    ret = bind(listen_fd_, (struct sockaddr*)&addr, sizeof(addr));
    iAssert(ret, ("bind: listen_fd:%d", listen_fd_));

    ret = listen(listen_fd_, 1024);
    iAssert(ret, ("listen: listen_fd:%d", listen_fd_));

    // SSL
    SSLConnector connector(SSL_CRT_SERVER, SSL_KEY_SERVER, SSL_SELECT_SERVER);
    struct sockaddr_in cli_addr;

    socklen_t cli_len = sizeof(cli_addr);
    int tmp_fd = accept(listen_fd_, (struct sockaddr*)&cli_addr, &cli_len);
    TLOG_DBG(("accept listen_fd:%d, tmp_fd:%d, errno:%d, errmsg:%s", listen_fd_, tmp_fd, errno, strerror(errno)));

    ret = connector.SSLAccept(tmp_fd);
    iAssert(ret, ("SSLAccept"));

    return 0;
}