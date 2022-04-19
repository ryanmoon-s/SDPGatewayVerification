#pragma once

#include "comm/commdef/comm_def.h"
#include "comm/tlog/tlog.h"
#include "comm/spatools/spa_tool.h"
#include "comm/ssltools/ssl_tool.h"
#include "comm/erpc/erpc_client.h"

#include <vector>

using std::vector;
using std::pair;

// SPA敲门后 进行TLS连接 等待时长
#define SPA_WAIT_TIME 3

class VerifyClient {
public:
    // 与SDPController交互
    // 获取可访问的应用列表: {<IP, PORT>}
    int GetAccessibleAppList( /*vector<pair<std::string, int>>& list, */ const spa::SPAVoucher& vocher);

private:
    // SPA敲门，使用UDP进行交互
    int _SPAKnocking(const spa::SPAPacket& packet, std::string ip, int port);
};

class AccessClient {
    // 与AppGateway交互
    // 访问应用
    int AccessApplication();

};
