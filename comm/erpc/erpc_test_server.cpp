#include "erpc_server.h"
#include "comm/commdef/comm_def.h"

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