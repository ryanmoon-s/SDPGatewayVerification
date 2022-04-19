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

        // APPGATEWAY
        CMD_UDP_APPGATEWAY_FUNC_RECV = 201,
    };
    
}

class ErpcService {
public:

    /***************************** TEST FUNC *****************************/
    
    virtual int TestFuncReverse(const erpc::TestFuncReverseReq& objReq, erpc::TestFuncReverseRsp& objRsp);

    virtual int TestFuncUdpRecv(const std::string& msg);

    /***************************** CONTROLLER *****************************/

    virtual int ControllerFuncUdpRecv(const std::string& msg);

    /***************************** APPGATEWAY *****************************/

    virtual int AppGatewayFuncUdpRecv(const std::string& msg);


public:
    ~ErpcService() {}
};
