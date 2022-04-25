#include "erpc_service.h"

/***************************** TEST FUNC *****************************/

int ErpcService::TestFuncReverse(const erpc::TestFuncReverseReq& objReq, erpc::TestFuncReverseRsp& objRsp, const erpc::Extra& extra)
{
    return erpc::kErrServiceNotFound;
}

int ErpcService::TestFuncUdpRecv(const std::string& msg, std::string ip, int port)
{
    return erpc::kErrServiceNotFound;
}

/***************************** CONTROLLER *****************************/

int ErpcService::ConFuncUdpRecv(const std::string& msg, std::string from_ip, int from_port)
{
    return erpc::kErrServiceNotFound;
}

int ErpcService::ConFuncGetAccess(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp, const erpc::Extra& extra)
{
    return erpc::kErrServiceNotFound;
}

int ErpcService::ConFuncRegisterApp(const erpc::ConFuncRegisterAppReq& objReq, erpc::ConFuncRegisterAppRsp& objRsp, const erpc::Extra& extra)
{
    return erpc::kErrServiceNotFound;
}

/***************************** APPGATEWAY *****************************/

int ErpcService::GateFuncUdpRecv(const std::string& msg, std::string from_ip, int from_port)
{
    return erpc::kErrServiceNotFound;
}

int ErpcService::GateFuncBlackListOp(const erpc::GateFuncBlackListOpReq& objReq, erpc::GateFuncBlackListOpRsp& objRsp, const erpc::Extra& extra)
{
    return erpc::kErrServiceNotFound;
}

/***************************** APPGATEWAY *****************************/

int ErpcService::AppFuncHttps(const std::string& request, std::string& response)
{
    return erpc::kErrServiceNotFound;
}
