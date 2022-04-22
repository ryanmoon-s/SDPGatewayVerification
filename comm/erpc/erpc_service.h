#pragma once

#include "comm/tlog/tlog.h"
#include "comm/proto/erpc.pb.h"
#include "comm/proto/spa.pb.h"
#include "erpc_def.h"

namespace erpc {
    enum ServiceMapTable {
        // TEST
        CMD_UDP_TEST_FUNC_RECV = 1,
        CMD_RPC_TEST_FUNC_REVERSE = 2,

        // CONTROLLER
        CMD_UDP_CONTROLLER_FUNC_RECV = 101,
        CMD_RPC_CONTROLLER_FUNC_GET_ACCESS = 102,

        // APPGATEWAY
        CMD_UDP_APPGATEWAY_FUNC_RECV = 201,
        CMD_RPC_APPGATEWAY_FUNC_WHITE_LIST_OP = 202,
    };
    
}

class ErpcService {
public:

    /***************************** TEST FUNC *****************************/

    virtual int TestFuncUdpRecv(const std::string& msg, std::string ip, int port);
    
    virtual int TestFuncReverse(const erpc::TestFuncReverseReq& objReq, erpc::TestFuncReverseRsp& objRsp);

    /***************************** CONTROLLER *****************************/

    virtual int ConFuncUdpRecv(const std::string& msg, std::string ip, int port);

    virtual int ConFunGetAccess(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp);

    /***************************** APPGATEWAY *****************************/

    virtual int GateFuncUdpRecv(const std::string& msg, std::string ip, int port);

    virtual int GateFuncWhiteListOp(const erpc::GateFuncWhiteListOpReq& objReq, erpc::GateFuncWhiteListOpRsp& objRsp);

public:
    ~ErpcService() {}
};
