#ifndef EPOLL_DISPATCHER_H
#define EPOLL_DISPATCHER_H

#include <map>
#include "erpc_def.h"
#include "erpc_handler.h"

#define MAXEVENTS 128

struct FdDataType {
    int fd;
    uint32_t event_type;
    std::shared_ptr<SSLConnector> connector;
};

struct EpollData {
    int epollfd = 0;
    std::map<int, FdDataType> fdmap;
    struct epoll_event *events;
};

class EpollDispatcher {
public:
    int DispatcherAdd(const FdDataType& fd_data);
    int DispatcherDel(const FdDataType& fd_data);
    int DispatcherMod(const FdDataType& fd_data);
    int Dispatch();

    // 产生一对本地套接字，一个添加到Epoll，一个用于本地通信。
    int GenerateLocalSocket(int& wr_fd);

public:
    EpollDispatcher(int port);
    ~EpollDispatcher();

private:
    int _MakeListenFd(int port);
    int _MakeUdpFd(int port);

private:
    int local_fd_ = 0;
    int listen_fd_ = 0;
    int udp_fd_ = 0;
    EpollData epoll_data_;
};




#endif