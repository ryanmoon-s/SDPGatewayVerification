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
        // TCP
        CMD_TCP_TEST_FUNC_REVERSE = 101,

        // UDP
        CMD_UDP_TEST_FUNC_RECV = 102,
    };

    /***************************** TEST FUNC *****************************/
    
    virtual int TestFuncReverse(const erpc::TestFuncReverseReq& objReq, erpc::TestFuncReverseRsp& objRsp);
    
    virtual int TestFuncUdpRecv(const spa::SPAVoucher& spaVoucher);

    /***************************** CONTROLLER *****************************/


    /***************************** APPLICATION *****************************/



public:
    ~ErpcService() {}
};


#endif