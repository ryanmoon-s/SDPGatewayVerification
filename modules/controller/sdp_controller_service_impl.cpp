#include "comm/erpc/erpc_client.h"
#include "comm/proto/spa.pb.h"
#include "comm/proto/erpc.pb.h"

#include "sdp_controller_service_impl.h"
#include "sdp_controller_tool.h"
#include "sdp_controller_config.h"

int SDPControllerErpcServiceImpl::ConFuncUdpRecv(const std::string& msg, std::string ip, int port)
{
    int ret = 0;
    spa::SPAVoucherPacket spaVoucherPacket;
    spa::SPAVoucher spaVoucher;
    auto config = SDPControllerConfig::GetInstance();

    // 解出 spaVoucher
    spaVoucherPacket.ParseFromString(msg);
    ret = SPATools().DecryptVoucher(spaVoucher, spaVoucherPacket);
    iAssert(ret, ("DecryptVoucher faild"));

    // 输出 spaVoucher 检查日志
    std::string voucher_str;
    spaVoucher.SerializeToString(&voucher_str);
    TLOG_DBG(("spaVoucher: %s", voucher_str.c_str()));

    // TODO iAssert
    ret = SDPControllerTool().CheckUserPermissions(spaVoucher);
    if (ret != 0)
    {
        TLOG_MSG(("CheckUserPermissions faild, spaVoucher data:%s", voucher_str.c_str()));
        return -1;
    }

    // 防止重放攻击
    int repeat = config->QueryAndInsertMD5(spaVoucherPacket.md5_data());
    if (repeat != 0)
    {
        TLOG_MSG(("QueryAndInsertMD5 repeat md5"));
        return -1;
    }

    // 加入白名单
    config->GetWhiteListObj()->OpWhiteList(IP_WHITE_TABLE_ADD, ip, port);
    
    return 0;
}

int SDPControllerErpcServiceImpl::ConFuncGetAccess(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp, const erpc::Extra& extra)
{
    int ret = 0;
    erpc::SocketInfo socket_info = extra.socket_info;

    // 派发 ticket
    spa::SPATicket spaTicket;
    spaTicket.set_ip(socket_info.ip.c_str());
    spaTicket.set_timestamp(time(NULL));

    spa::SPATicketPacket spaTicketPacket;
    ret = SPATools().EncryptVoucher(spaTicketPacket, spaTicket);
    iAssert(ret, ("EncryptVoucher faild"));

    objRsp.mutable_ticket_packet()->CopyFrom(spaTicketPacket);

    // 添加 Access List
    

    return 0;
}

int SDPControllerErpcServiceImpl::ConFuncRegisterApp(const erpc::ConFuncRegisterAppReq& objReq, erpc::ConFuncRegisterAppRsp& objRsp, const erpc::Extra& extra)
{
    
}