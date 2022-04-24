#pragma once

#include "comm/commdef/comm_def.h"
#include "comm/spatools/spa_tool.h"
#include "comm/ssltools/ssl_tool.h"
#include "comm/proto/erpc.pb.h"
#include "comm/erpc/erpc_client.h"
#include <vector>

// SPA敲门后 进行TLS连接 等待时长
#define SPA_WAIT_TIME 3

class VerifyClient {
public:
    VerifyClient() :erpc_client_(SSL_CRT_CLIENT, SSL_KEY_CLIENT) {}

public:
    // 1、从Controller获取可访问的应用列表: <IP, UDP_PORT, TCP_PORT, APP, -Ticket->
    // 2、对APP进行敲门
    int GetAccessibleAppList(std::vector<erpc::AccessItem>& list, const spa::SPAVoucher &spaVoucher);

private:
    // SPA敲门，使用UDP进行交互
    int _SPAKnockingController(const spa::SPAVoucher &spaVoucher, std::string ip, int port);
    int _SPAKnockingGateway(const spa::SPATicketPacket &spaTicketPacket, std::string ip, int port);

private:
    ErpcClient erpc_client_;
};



class AccessClient {
public:
    AccessClient() :erpc_client_(SSL_CRT_CLIENT, SSL_KEY_CLIENT) {}

public:
    // 访问应用
    int AccessApplication_HTTPS(const std::vector<erpc::AccessItem>& list);

private:
    ErpcClient erpc_client_;
};
