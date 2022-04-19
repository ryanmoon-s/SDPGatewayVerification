#include "sdp_controller_service_impl.h"
#include "comm/erpc/erpc_client.h"

int SDPControllerErpcServiceImpl::ControllerFuncUdpRecv(const std::string& msg)
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

    // TEST
    erpc_def::Header header;
    erpc::GateFuncWhiteListOpReq req;
    erpc::GateFuncWhiteListOpRsp rsp;

    req.set_op(erpc::ListAdd);
    req.set_ip("127.0.0.1"); 

    ret = ErpcClient().GateFuncWhiteListOpRequest(req, rsp, header);
    iAssert(ret, ("GateFuncWhiteListOpRequest faild"));

    TLOG_DBG(("header code:%d, msg:%s", header.ret_code, header.ret_msg.c_str()));

    return 0;
}