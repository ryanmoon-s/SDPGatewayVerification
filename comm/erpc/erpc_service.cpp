#include "erpc_service.h"

int ErpcService::TestFuncReverse(const erpc::TestFuncReverseReq& objReq, erpc::TestFuncReverseRsp& objRsp)
{
    return erpc_def::kErrServiceNotFound;
}

int ErpcService::TestFuncUdpRecv(const std::string& msg)
{
    return erpc_def::kErrServiceNotFound;
}
