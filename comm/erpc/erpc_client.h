#pragma once

#include "erpc_def.h"
#include "erpc_handler.h"
#include "erpc_service.h"

#include "comm/proto/erpc.pb.h"

using namespace erpc;

class ErpcClient {
public:
    /***************************** UDP *****************************/
    int UDPFuncRequest(uint32_t cmdid, const std::string& msg, const std::string& ip, int port);

    /***************************** RPC *****************************/

    // Test
    int TestFuncReverseRequest(const erpc::TestFuncReverseReq& req, erpc::TestFuncReverseRsp& rsp, Header& header);

    // Controller
    int ConFuncGetAccessRequest(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp, Header& header);
    int ConFuncRegisterAppRequest(const erpc::ConFuncRegisterAppReq& objReq, erpc::ConFuncRegisterAppRsp& objRsp, Header& header);

    // AppGateway
    int GateFuncWhiteListOpRequest(const erpc::GateFuncWhiteListOpReq& objReq, erpc::GateFuncWhiteListOpRsp& objRsp, Header& header);

};
