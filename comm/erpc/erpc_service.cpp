#include "erpc_service.h"

/***************************** TEST FUNC *****************************/

int ErpcService::TestFuncReverse(const erpc::TestFuncReverseReq& objReq, erpc::TestFuncReverseRsp& objRsp)
{
    return erpc_def::kErrServiceNotFound;
}

int ErpcService::TestFuncUdpRecv(const std::string& msg, std::string ip, int port)
{
    return erpc_def::kErrServiceNotFound;
}

/***************************** CONTROLLER *****************************/

int ErpcService::ConFuncUdpRecv(const std::string& msg, std::string ip, int port)
{
    return erpc_def::kErrServiceNotFound;
}

int ErpcService::ConFunGetAccess(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp)
{
    return erpc_def::kErrServiceNotFound;
}

/***************************** APPGATEWAY *****************************/

int ErpcService::GateFuncUdpRecv(const std::string& msg, std::string ip, int port)
{
    return erpc_def::kErrServiceNotFound;
}

int ErpcService::GateFuncWhiteListOp(const erpc::GateFuncWhiteListOpReq& objReq, erpc::GateFuncWhiteListOpRsp& objRsp)
{
    return erpc_def::kErrServiceNotFound;
}
