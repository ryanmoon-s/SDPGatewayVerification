#pragma once

#include "erpc_def.h"
#include "erpc_handler.h"
#include "erpc_service.h"

#include "comm/proto/erpc.pb.h"

using namespace erpc_def;

class ErpcClient {
public:
    /***************************** UDP *****************************/
    int UDPFuncRequest(uint32_t cmdid, const std::string& msg, const std::string& ip, int port);

    /***************************** RPC *****************************/

    // Test
    int TestFuncReverseRequest(const erpc::TestFuncReverseReq& req, erpc::TestFuncReverseRsp& rsp, Header& header);

    // Controller
    int ConFunGetAccessRequest(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp);

    // AppGateway
    int GateFuncWhiteListOpRequest(const erpc::GateFuncWhiteListOpReq& objReq, erpc::GateFuncWhiteListOpRsp& objRsp, Header& header);

};
