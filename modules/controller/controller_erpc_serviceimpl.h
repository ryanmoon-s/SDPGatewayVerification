#pragma once

#include "comm/erpc/erpc_service.h"
#include "comm/spatools/spa_tool.h"
#include "controller_tool.h"

class SDPControllerErpcServiceImpl: public ErpcService {
public:

    virtual int ControllerFuncUdpRecv(const std::string& msg) override;


};
