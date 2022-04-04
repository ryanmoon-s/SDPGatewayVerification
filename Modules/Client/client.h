#ifndef CLIENT_H
#define CLIENT_H

#include "Comm/CommDef/comm_def.h"
#include "Comm/TLog/tlog.h"

class Client {
public:
    // 与SDPController交互，获取通行票据
    int GetTicket();

    // 建立SSL加密通道连接
    int SSL_Connect(std::string ip, int port);

    // 在SSL加密通道下，同步消息
    int SSL_SyncMsg(std::string msg);
};














#endif
