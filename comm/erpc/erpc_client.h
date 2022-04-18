#ifndef ERPC_CLIENT_H
#define ERPC_CLIENT_H

#include "erpc_def.h"
#include "erpc_handler.h"

#include "comm/proto/erpc.pb.h"

using namespace erpc_def;

class Client {
public:
    int FuncReverseClient(const erpc::FuncReverseReq& req, erpc::FuncReverseRsp& rsp, Header& header);

};


#endif