#ifndef ERPC_CLIENT_H
#define ERPC_CLIENT_H

#include "erpc_def.h"
#include "erpc_handler.h"

#include "comm/proto/erpc.pb.h"

using namespace erpc_def;
using namespace erpc_service;

class Client {
public:
    int TestFuncClient(const erpc::TestFuncReq& req, erpc::TestFuncRsp& rsp, Header& header);

};


#endif