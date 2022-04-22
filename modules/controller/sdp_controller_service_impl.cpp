#include "comm/erpc/erpc_client.h"
#include "sdp_controller_service_impl.h"
#include "sdp_controller_tool.h"
#include "sdp_controller_config.h"

int SDPControllerErpcServiceImpl::ConFuncUdpRecv(const std::string& msg, std::string ip, int port)
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

    // TODO iAssert
    ret = CheckUserPermissions(spaVoucher);
    if (ret != 0)
    {
        TLOG_MSG(("CheckUserPermissions faild, voucher data:%s", voucher_str.c_str()));
        return -1;
    }

    // 防止重放攻击
    int repeat = QueryAndInsertMD5(spaVoucherPacket.md5_data());
    if (repeat != 0)
    {
        TLOG_MSG(("QueryAndInsertMD5 repeat md5"));
        return -1;
    }

    // 加入白名单
    auto config = SDPControllerConfig::GetInstance();
    config->GetWhiteListObj()->OpWhiteList(IP_WHITE_TABLE_ADD, ip, port);
    
    return 0;
}
