#pragma once

#include "comm/erpc/erpc_service.h"
#include "comm/spatools/spa_tool.h"
#include "sdp_gateway_tool.h"
#include "sdp_gateway_config.h"

class SDPAppGatewayErpcServiceImpl: public ErpcService {
public:

    virtual int GateFuncUdpRecv(const std::string& msg, std::string from_ip, int from_port) override;

    virtual int GateFuncBlackListOp(const erpc::GateFuncBlackListOpReq& objReq, erpc::GateFuncBlackListOpRsp& objRsp, const erpc::Extra& extra) override;

};
