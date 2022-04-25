#pragma once

#include "comm/erpc/erpc_service.h"
#include "comm/spatools/spa_tool.h"

class SDPControllerErpcServiceImpl: public ErpcService {
public:

    virtual int ConFuncUdpRecv(const std::string& msg, std::string from_ip, int from_port) override;

    virtual int ConFuncGetAccess(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp, const erpc::Extra& extra) override;

    virtual int ConFuncRegisterApp(const erpc::ConFuncRegisterAppReq& objReq, erpc::ConFuncRegisterAppRsp& objRsp, const erpc::Extra& extra);

};
