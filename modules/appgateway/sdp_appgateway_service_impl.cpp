#include "sdp_appgateway_service_impl.h"

int SDPAppGatewayErpcServiceImpl::GateFuncUdpRecv(const std::string& msg)
{
    int ret = 0;
    spa::SPAPacket spaPacket;
    spa::SPAVoucher spaVoucher;

    // 解出 spaVoucher
    spaPacket.ParseFromString(msg);
    ret = SPATools().DecryptVoucher(spaVoucher, spaPacket);
    iAssert(ret, ("DecryptVoucher faild"));

    // 输出 spaVoucher 检查日志
    std::string voucher_str;
    spaVoucher.SerializeToString(&voucher_str);
    TLOG_DBG(("spaVoucher: %s", voucher_str.c_str()));

    //


    return 0;
}

int SDPAppGatewayErpcServiceImpl::GateFuncWhiteListOp(const erpc::GateFuncWhiteListOpReq& objReq, erpc::GateFuncWhiteListOpRsp& objRsp)
{


    TLOG_MSG(("GateFuncWhiteListOp success, op:%d, ip:%s", objReq.op(), objReq.ip().c_str()));
    return 0;
}
