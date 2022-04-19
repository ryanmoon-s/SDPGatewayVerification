#pragma once

#include "comm/erpc/erpc_service.h"
#include "comm/spatools/spa_tool.h"
#include "sdp_appgateway_tool.h"

class SDPAppGatewayErpcServiceImpl: public ErpcService {
public:

    virtual int AppGatewayFuncUdpRecv(const std::string& msg) override;


};
