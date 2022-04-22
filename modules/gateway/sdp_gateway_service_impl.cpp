#include "sdp_gateway_service_impl.h"

int SDPAppGatewayErpcServiceImpl::GateFuncUdpRecv(const std::string& msg, std::string ip, int port)
{
    int ret = 0;
    spa::SPAVoucherPacket spaVoucherPacket;
    spa::SPAVoucher spaVoucher;

    // 解出 spaVoucher
    spaVoucherPacket.ParseFromString(msg);
    ret = SPATools().DecryptVoucher(spaVoucher, spaVoucherPacket);
    iAssert(ret, ("DecryptVoucher faild"));

    // 输出 spaVoucher 检查日志
    std::string voucher_str;
    spaVoucher.SerializeToString(&voucher_str);
    TLOG_DBG(("spaVoucher: %s", voucher_str.c_str()));

    //


    return 0;
}

int SDPAppGatewayErpcServiceImpl::GateFuncWhiteListOp(const erpc::GateFuncWhiteListOpReq& objReq, erpc::GateFuncWhiteListOpRsp& objRsp, const erpc::Extra& extra)
{
    auto config = SDPAppGatewayConfig::GetInstance();
    auto whitelist = config->GetWhiteListObj();
    whitelist->OpWhiteList(objReq.op(), objReq.ip(), config->get_tcp_port());

    TLOG_MSG(("GateFuncWhiteListOp success, op:%d, ip:%s", objReq.op(), objReq.ip().c_str()));
    return 0;
}
