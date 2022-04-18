#ifndef ERPC_CLIENT_H
#define ERPC_CLIENT_H

#include "erpc_def.h"
#include "erpc_handler.h"
#include "erpc_service.h"

#include "comm/proto/erpc.pb.h"

using namespace erpc_def;

class ErpcClient {
public:
    int TestFuncReverseClient(const erpc::TestFuncReverseReq& req, erpc::TestFuncReverseRsp& rsp, Header& header);
    int TestFuncUdpRecv(uint32_t cmdid, const std::string& msg, const std::string& ip, int port);

};


#endif