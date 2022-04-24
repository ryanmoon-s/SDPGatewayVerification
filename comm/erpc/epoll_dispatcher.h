#pragma once

#include <map>
#include "erpc_def.h"
#include "erpc_handler.h"

#define MAXEVENTS 128

struct FdDataType {
    int fd;
    uint32_t event_type;
    erpc::SocketInfo socket_info;
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
    int Dispatch();

public:
    EpollDispatcher();
    // controller gateway
    EpollDispatcher(const std::string& ip, int tcp_port, int udp_port);
    // application
    EpollDispatcher(const std::string& ip, int tcp_port);

    ~EpollDispatcher();

private:
    int _MakeListenFd(int& fd, const std::string& ip, int port);
    int _MakeUdpFd(int& fd, const std::string& ip, int port);

private:
    // controller:rpc
    // gateway:rpc
    // application:tcp
    int listen_fd_; 

    // controller:udp
    // gateway:udp
    int udp_fd_;

    int application_port_;
    
    EpollData epoll_data_;
};
