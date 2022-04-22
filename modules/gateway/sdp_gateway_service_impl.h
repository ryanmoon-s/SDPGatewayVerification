#pragma once

#include "comm/erpc/erpc_service.h"
#include "comm/spatools/spa_tool.h"
#include "sdp_gateway_tool.h"
#include "sdp_gateway_config.h"

class SDPAppGatewayErpcServiceImpl: public ErpcService {
public:

    virtual int GateFuncUdpRecv(const std::string& msg, std::string ip, int port) override;

    virtual int GateFuncWhiteListOp(const erpc::GateFuncWhiteListOpReq& objReq, erpc::GateFuncWhiteListOpRsp& objRsp) override;

};
