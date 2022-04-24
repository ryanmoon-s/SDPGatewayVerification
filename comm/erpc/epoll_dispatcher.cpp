#include "epoll_dispatcher.h"

EpollDispatcher::EpollDispatcher()
{
    int ret = 0;
    ret = DispatcherInit();
    iAssertNoReturn(ret, ("DispatcherInit faild"));
}

// controller gateway
EpollDispatcher::EpollDispatcher(const std::string& ip, int tcp_port, int udp_port)
    : EpollDispatcher()
{
    int ret = 0;
    ret = _MakeListenFd(listen_fd_, ip, tcp_port);
    iAssertNoReturn(ret, ("_MakeListenFd faild"));

    ret = _MakeUdpFd(udp_fd_, ip, udp_port);
    iAssertNoReturn(ret, ("_MakeUdpFd faild"));
}

// application
EpollDispatcher::EpollDispatcher(const std::string& ip, int tcp_port)
    : EpollDispatcher()
{
    application_port_ = tcp_port;
    int ret = _MakeListenFd(listen_fd_, ip, tcp_port);
    iAssertNoReturn(ret, ("_MakeListenFd faild"));
}

EpollDispatcher::~EpollDispatcher() 
{
    free(epoll_data_.events);
    close(epoll_data_.epollfd);
    epoll_data_.fdmap.clear();
}

int EpollDispatcher::DispatcherInit()
{
    epoll_data_.epollfd = epoll_create1(0);
    iAssert(epoll_data_.epollfd, ("epoll_create1 faild"));

    epoll_data_.events = (struct epoll_event*)calloc(MAXEVENTS, sizeof(struct epoll_event));
    iAssertNull(epoll_data_.events, ("calloc epoll_data_.events faild"));

    return 0;
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
            TLOG_MSG(("=== ===  === Event Beg === === ==="));
            TLOG_MSG(("epoll in listen_fd_:%d, wakeup_fd:%d", listen_fd_, fd));
            FdDataType fd_data;

            // UDP READ
            if (fd == udp_fd_)
            {
                fd_data = epoll_data_.fdmap[fd];
                ret = handler.HandleUDPRequest(fd_data);
                iAssertNoReturn(ret, ("HandleUDPRequest faild, fd:%d", fd));
            }

            // TCP Accept
            else if (fd == listen_fd_)
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

            // TCP READ
            else
            {
                fd_data = epoll_data_.fdmap[fd];
                int port = fd_data.socket_info.port;

                // Application
                if (port == application_port_)
                {
                    ret = handler.HandleApplicationRequest(fd_data);
                    iAssertNoReturn(ret, ("HandleApplicationRequest faild, fd:%d", fd));
                }
                // RPC
                else 
                {
                    ret = handler.HandleRPCRequest(fd_data);
                    iAssertNoReturn(ret, ("HandleRPCRequest faild, fd:%d", fd));

                    // Keep Alive TODO
                    ret = DispatcherDel(fd_data);
                    iAssertNoReturn(ret, ("DispatcherDel faild, fd:%d", fd));
                }
            }

            TLOG_MSG(("=== ===  === Event End === === ==="));
        }

        if (type & EPOLLOUT) 
        {
           TLOG_DBG(("epoll out fd:%d", fd));
        }
    }

    return 0;
}

int EpollDispatcher::_MakeListenFd(int& fd, const std::string& ip, int port)
{
    int ret = 0, resue = 1;
    struct sockaddr_in addr;
    FdDataType fd_data;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    
    fd = socket(AF_INET, SOCK_STREAM, 0); 
    iAssert(fd, ("socket: listen_fd:%d", fd));

    ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&resue, sizeof(int));
    iAssert(ret, ("setsockopt: listen_fd:%d", fd));

    ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    iAssert(ret, ("bind: listen_fd:%d", fd));

    ret = listen(fd, 1024);
    iAssert(ret, ("listen: listen_fd:%d", fd));

    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK); 

    fd_data.fd = fd;
    fd_data.event_type = EPOLLIN | EPOLLET;

    ret = DispatcherAdd(fd_data);
    iAssert(ret, ("DispatcherAdd fd:%d", fd));

    return 0;
}

int EpollDispatcher::_MakeUdpFd(int& fd, const std::string& ip, int port)
{
    int ret = 0;
    FdDataType fd_data;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    iAssert(fd, ("socket udp_fd:%d", fd));

    struct sockaddr_in addr;
 
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
 
	ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    iAssert(ret, ("bind udp_fd:%d", fd));

    fd_data.fd = fd;
    fd_data.event_type = EPOLLIN | EPOLLET;

    ret = DispatcherAdd(fd_data);
    iAssert(ret, ("DispatcherAdd udp_fd:%d", fd));

    return 0;
}
