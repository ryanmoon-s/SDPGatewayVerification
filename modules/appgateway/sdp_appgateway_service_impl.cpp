#include "sdp_appgateway_service_impl.h"

int SDPAppGatewayErpcServiceImpl::AppGatewayFuncUdpRecv(const std::string& msg)
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