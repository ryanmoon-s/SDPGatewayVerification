#pragma once

#include "comm/erpc/erpc_service.h"
#include "comm/spatools/spa_tool.h"

class SDPControllerErpcServiceImpl: public ErpcService {
public:

    virtual int ConFuncUdpRecv(const std::string& msg, std::string ip, int port) override;


};
