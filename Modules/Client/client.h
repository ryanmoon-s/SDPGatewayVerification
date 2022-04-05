#ifndef CLIENT_H
#define CLIENT_H

#include "Comm/CommDef/comm_def.h"
#include "Comm/CommDef/controller_def.h"
#include "Comm/TLog/tlog.h"
#include "Comm/SPATools/spa_tool.h"

class Client {
public:
    // 与SDPController交互
    // 获取可访问的应用列表: {<IP, PORT>}
    int GetAccessibleAppList(set<ACC_LIST_TYPE>& list, const SPAVoucher& vocher);

    // 与AppGateway交互
    // 访问应用
    int AccessApplication();
};














#endif
