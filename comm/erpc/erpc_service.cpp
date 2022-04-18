#include "erpc_service.h"

int ErpcService::TestFuncReverse(const erpc::TestFuncReverseReq& objReq, erpc::TestFuncReverseRsp& objRsp)
{
    return kErrServiceNotFound;
}

int ErpcService::TestFuncUdpRecv(const spa::SPAVoucher& spaVoucher)
{
    return kErrServiceNotFound;
}
