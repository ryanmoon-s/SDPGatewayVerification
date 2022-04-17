#ifndef ERPC_SERVICE
#define ERPC_SERVICE

#include "comm/tlog/tlog.h"
#include "comm/proto/erpc.pb.h"
#include "erpc_def.h"

using namespace erpc_def;

namespace erpc_service 
{
    // rpc service table
    enum ServiceMapTable {
        CMD_FUNC_REVERSE = 101,
    };

    // rpc service
    int FuncReverse(const erpc::FuncReverseReq& objReq, erpc::FuncReverseRsp& objRsp);




}


#endif