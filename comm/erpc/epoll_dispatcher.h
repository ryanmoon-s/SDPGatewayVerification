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
    int DispatcherInit();
    int DispatcherAdd(const FdDataType& fd_data);
    int DispatcherDel(const FdDataType& fd_data);
    int DispatcherMod(const FdDataType& fd_data);
    int Dispatch(int listen_fd, const std::map<std::string, int>& ip_white_table);

public:
    EpollDispatcher();
    ~EpollDispatcher();
private:
    EpollData epoll_data_;
};




#endif