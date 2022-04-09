#ifndef CLIENT_H
#define CLIENT_H

#include "Comm/CommDef/comm_def.h"
#include "Comm/CommDef/controller_def.h"
#include "Comm/TLog/tlog.h"
#include "Comm/SPATools/spa_tool.h"
#include "Comm/SSLTools/ssl_tool.h"

#include <vector>

using std::vector;
using std::pair;

#define SPA_WAIT_TIME 3

class Client {
public:
    // 与SDPController交互
    // 获取可访问的应用列表: {<IP, PORT>}
    int GetAccessibleAppList(vector<pair<std::string, int>>& list, const SPAVoucher& vocher);

    // 与AppGateway交互
    // 访问应用
    int AccessApplication();

    // TOOL
    // SPA敲门，使用UDP进行交互
    int SPAKnocking(const SPAPacket& packet, std::string ip, int port);
};














#endif
