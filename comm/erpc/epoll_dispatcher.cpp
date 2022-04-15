#include "epoll_dispatcher.h"

int EpollDispatcher::DispatcherInit() 
{
    epoll_data_.epollfd = epoll_create1(0);
    iAssert(epoll_data_.epollfd, ("epoll_create1"));

    epoll_data_.events = (struct epoll_event*)calloc(MAXEVENTS, sizeof(struct epoll_event));
    iAssertNull(epoll_data_.events, ("calloc"));

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

int EpollDispatcher::Dispatch(int listen_fd) 
{
    int nums = epoll_wait(epoll_data_.epollfd, epoll_data_.events, MAXEVENTS, -1);
    iAssert(nums, ("epoll_wait faild, epollfd:%d, errno:%d, errmsg:%s", epoll_data_.epollfd, errno, strerror(errno)));

    TLOG_DBG(("epoll_wait wakeup, epollfd:%d nums:%d", epoll_data_.epollfd, nums));

    for (int i = 0; i < nums; i++) {
        int fd = epoll_data_.events[i].data.fd;
        uint32_t type = epoll_data_.events[i].events;

        if ((type & EPOLLERR) || (type & EPOLLHUP)) 
        {
            TLOG_DBG(("epoll error, fd:%d", fd));
            close(fd);
            continue;
        }

        if (type & EPOLLIN) 
        {
            TLOG_DBG(("epoll read fd:%d", fd));
            FdDataType fd_data;
            if (fd == listen_fd)
            {
                ErpcHandler().HandleNetAccept(listen_fd, fd_data);
                this->DispatcherAdd(fd_data);
            }
            else
            {
                fd_data = epoll_data_.fdmap[fd];
                ErpcHandler().HandleNetRquest(fd_data);
            }
        }

        if (type & EPOLLOUT) 
        {
           TLOG_DBG(("epoll write fd:%d", fd));
        }
    }

    return 0;
}

EpollDispatcher::EpollDispatcher()
{
    DispatcherInit();
}

EpollDispatcher::~EpollDispatcher() 
{
    free(epoll_data_.events);
    close(epoll_data_.epollfd);
    epoll_data_.fdmap.clear();
}
