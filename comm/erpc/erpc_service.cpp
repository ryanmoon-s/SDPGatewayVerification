#include "erpc_service.h"

int ErpcService::TestFuncReverse(const erpc::TestFuncReverseReq& objReq, erpc::TestFuncReverseRsp& objRsp)
{
    return erpc_def::kErrServiceNotFound;
}

int ErpcService::TestFuncUdpRecv(const std::string& msg)
{
    return erpc_def::kErrServiceNotFound;
}

int ErpcService::ControllerFuncUdpRecv(const std::string& msg)
{
    return erpc_def::kErrServiceNotFound;
}

int ErpcService::GateFuncUdpRecv(const std::string& msg)
{
    return erpc_def::kErrServiceNotFound;
}

int ErpcService::GateFuncWhiteListOp(const erpc::GateFuncWhiteListOpReq& objReq, erpc::GateFuncWhiteListOpRsp& objRsp)
{
    return erpc_def::kErrServiceNotFound;
}
