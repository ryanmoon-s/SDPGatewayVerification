#ifndef ERPC_SERVICE
#define ERPC_SERVICE

#include "comm/tlog/tlog.h"
#include "comm/proto/erpc.pb.h"
#include "comm/proto/spa.pb.h"
#include "erpc_def.h"

using namespace erpc_def;

class ErpcService {
public:

    enum ServiceMapTable {
        // TEST
        CMD_RPC_TEST_FUNC_REVERSE = 1,
        CMD_UDP_TEST_FUNC_RECV = 2,

        // CONTROLLER
        CMD_UDP_CONTROLLER_FUNC_RECV = 101,

        // APPGATEWAY
        CMD_TCP_APPGATEWAY_FUNC_REVERSE = 201,
    };

    /***************************** TEST FUNC *****************************/
    
    virtual int TestFuncReverse(const erpc::TestFuncReverseReq& objReq, erpc::TestFuncReverseRsp& objRsp);
    
    virtual int TestFuncUdpRecv(const std::string& msg);

    /***************************** CONTROLLER *****************************/


    /***************************** APPGATEWAY *****************************/



public:
    ~ErpcService() {}
};


#endif