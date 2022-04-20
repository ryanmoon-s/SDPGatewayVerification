#include "epoll_dispatcher.h"

EpollDispatcher::EpollDispatcher(const std::string& ip, int tcp_port, int udp_port)
                                : listen_ip_(ip), tcp_port_(tcp_port), udp_port_(udp_port)
{
    epoll_data_.epollfd = epoll_create1(0);
    iAssertNoReturn(epoll_data_.epollfd, ("epoll_create1 faild"));

    epoll_data_.events = (struct epoll_event*)calloc(MAXEVENTS, sizeof(struct epoll_event));
    iAssertNoReturn(epoll_data_.events, ("calloc epoll_data_.events faild"));

    int ret = _MakeListenFd();
    iAssertNoReturn(ret, ("_MakeListenFd faild"));

    ret = _MakeUdpFd();
    iAssertNoReturn(ret, ("_MakeUdpFd faild"));
}

EpollDispatcher::~EpollDispatcher() 
{
    free(epoll_data_.events);
    close(epoll_data_.epollfd);
    epoll_data_.fdmap.clear();
}

int EpollDispatcher::DispatcherAdd(const FdDataType& fd_data) 
{
    int fd = fd_data.fd;
    uint32_t type = fd_data.event_type;

    struct epoll_event event;
    event.data.fd = fd;
    event.events = type;

    int ret = epoll_ctl(epoll_data_.epollfd, EPOLL_CTL_ADD, fd, &event);
    iAssert(ret, ("epoll_ctl add epollfd:%d fd:%d type:%u errno:%d errmsg:%s", epoll_data_.epollfd, fd, type, errno, strerror(errno)));

    epoll_data_.fdmap[fd] = fd_data;
    return 0;
}

int EpollDispatcher::DispatcherDel(const FdDataType& fd_data) 
{
    int fd = fd_data.fd;
    uint32_t type = fd_data.event_type;

    struct epoll_event event;
    event.data.fd = fd;
    event.events = type;
    int ret = epoll_ctl(epoll_data_.epollfd, EPOLL_CTL_DEL, fd, &event);
    iAssert(ret, ("epoll_ctl del epollfd:%d fd:%d type:%u", epoll_data_.epollfd, fd, type));

    close(fd);
    epoll_data_.fdmap.erase(fd);
    
    return 0;
}

int EpollDispatcher::DispatcherMod(const FdDataType& fd_data) 
{
    int fd = fd_data.fd;
    uint32_t type = fd_data.event_type;

    struct epoll_event event;
    event.data.fd = fd;
    event.events = type;

    int ret = epoll_ctl(epoll_data_.epollfd, EPOLL_CTL_MOD, fd, &event);
    iAssert(ret, ("epoll_ctl mod epollfd:%d fd:%d type:%u", epoll_data_.epollfd, fd, type));

    epoll_data_.fdmap[fd] = fd_data;
    return 0;
}

int EpollDispatcher::Dispatch() 
{
    int ret = 0, nums = 0;
    ErpcHandler handler;
    nums = epoll_wait(epoll_data_.epollfd, epoll_data_.events, MAXEVENTS, -1);
    iAssertNoReturn(nums, ("epoll_wait faild, epollfd:%d, errno:%d, errmsg:%s", epoll_data_.epollfd, errno, strerror(errno)));

    TLOG_DBG(("epoll wakeup, epollfd:%d nums:%d", epoll_data_.epollfd, nums));

    for (int i = 0; i < nums; i++) {
        int fd = epoll_data_.events[i].data.fd;
        uint32_t type = epoll_data_.events[i].events;

        if ((type & EPOLLERR) || (type & EPOLLHUP)) 
        {
            TLOG_ERR(("epoll error, erase fd:%d", fd));
            DispatcherDel(epoll_data_.fdmap[fd]);
        }

        if (type & EPOLLIN) 
        {
            TLOG_DBG(("epoll in listen_fd_:%d, wakeup_fd:%d", listen_fd_, fd));
            FdDataType fd_data;

            if (fd == listen_fd_)
            {
                ret = handler.HandleRPCAccept(listen_fd_, fd_data);
                
                if (ret == 0)
                {
                    ret = DispatcherAdd(fd_data);
                    iAssertNoReturn(ret, ("DispatcherAdd new_fd:%d", fd_data.fd));
                }
                else if (ret == kIpNotInWhiteTable)
                {
                    TLOG_MSG(("HandleRPCAccept faild, IP is illegal"));
                }
                else
                {
                    TLOG_ERR(("HandleRPCAccept faild"));
                }
            }
            else if (fd == local_fd_)
            {
                // 暂未使用
                // 使用前提：this->GenerateLocalSocket();
            }
            else if (fd == udp_fd_)
            {
                // UDP 请求
                fd_data = epoll_data_.fdmap[fd];
                ret = handler.HandleUDPRequest(fd_data);
                iAssertNoReturn(ret, ("HandleUDPRequest faild, fd:%d", fd));
            }
            else
            {
                fd_data = epoll_data_.fdmap[fd];
                ret = handler.HandleRPCRequest(fd_data);
                iAssertNoReturn(ret, ("HandleRPCRequest faild, fd:%d", fd));

                // Keep Alive TODO
                ret = DispatcherDel(fd_data);
                iAssertNoReturn(ret, ("DispatcherDel faild, fd:%d", fd));
            }
        }

        if (type & EPOLLOUT) 
        {
           TLOG_DBG(("epoll out fd:%d", fd));
        }
    }

    return 0;
}

int EpollDispatcher::GenerateLocalSocket(int& wr_fd)
{
    int sock_pair[2], ret = 0;
    FdDataType fd_data;

    ret = socketpair(AF_UNIX, SOCK_STREAM, 0, sock_pair);
    iAssert(ret, ("socketpair faild"));

    local_fd_ = sock_pair[0];
    wr_fd = sock_pair[1];

    fd_data.fd = local_fd_;
    fd_data.event_type = EPOLLIN;
    this->DispatcherAdd(fd_data);

    return 0;
}

int EpollDispatcher::_MakeListenFd()
{
    int ret = 0, resue = 1;
    struct sockaddr_in addr;
    FdDataType fd_data;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(tcp_port_);
    addr.sin_addr.s_addr = inet_addr(listen_ip_.c_str());
    
    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0); 
    iAssert(listen_fd_, ("socket: listen_fd_:%d", listen_fd_));

    ret = setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, (const void *)&resue, sizeof(int));
    iAssert(ret, ("setsockopt: listen_fd_:%d", listen_fd_));

    ret = bind(listen_fd_, (struct sockaddr*)&addr, sizeof(addr));
    iAssert(ret, ("bind: listen_fd_:%d", listen_fd_));

    ret = listen(listen_fd_, 1024);
    iAssert(ret, ("listen: listen_fd_:%d", listen_fd_));

    fcntl(listen_fd_, F_SETFL, fcntl(listen_fd_, F_GETFL, 0) | O_NONBLOCK); 

    fd_data.fd = listen_fd_;
    fd_data.event_type = EPOLLIN | EPOLLET;
    fd_data.connector = std::make_shared<SSLConnector>(SSL_CRT_SERVER, SSL_KEY_SERVER, 1);

    ret = DispatcherAdd(fd_data);
    iAssert(ret, ("DispatcherAdd listen_fd:%d", listen_fd_));

    return 0;
}

int EpollDispatcher::_MakeUdpFd()
{
    int ret = 0;
    FdDataType fd_data;

    udp_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    iAssert(udp_fd_, ("socket udp_fd:%d", udp_fd_));

    struct sockaddr_in addr;
 
	addr.sin_family = AF_INET;
	addr.sin_port = htons(udp_port_);
	addr.sin_addr.s_addr = inet_addr(listen_ip_.c_str());
 
	ret = bind(udp_fd_, (struct sockaddr*)&addr, sizeof(addr));
    iAssert(ret, ("bind udp_fd:%d", udp_fd_));

    fd_data.fd = udp_fd_;
    fd_data.event_type = EPOLLIN | EPOLLET;

    ret = DispatcherAdd(fd_data);
    iAssert(ret, ("DispatcherAdd udp_fd:%d", udp_fd_));
}
