#ifndef ERPC_SERVICE
#define ERPC_SERVICE

#include "comm/tlog/tlog.h"
#include "comm/proto/erpc.pb.h"
#include "comm/proto/spa.pb.h"
#include "erpc_def.h"

using namespace erpc_def;

class ErpcService {
public:
    // rpc service table
    enum ServiceMapTable {
        CMD_FUNC_REVERSE = 101,
    };

    // rpc service
    virtual int FuncReverse(const erpc::FuncReverseReq& objReq, erpc::FuncReverseRsp& objRsp) = 0;

    // udp service
    virtual int FuncUdpRecv(const spa::SPAVoucher& spaVoucher) = 0;

public:
    ~ErpcService() {}
};


#endif