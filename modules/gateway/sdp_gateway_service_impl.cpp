#include "sdp_gateway_service_impl.h"
#include "comm/commdef/comm_tool.h"

using namespace commtool;

int SDPAppGatewayErpcServiceImpl::GateFuncUdpRecv(const std::string& msg, std::string from_ip, int from_port)
{
    TLOG_MSG(("GateFuncUdpRecv begin size:%d", msg.size()));
    int ret = 0;
    spa::SPATicketPacket spaTicketPacket;
    auto config = SDPAppGatewayConfig::GetInstance();

    // 拦截Controller下发黑名单
    bool isblack = config->ControllerBlackList_IsIn(from_ip, config->get_app_tcp_port());
    if (isblack)
    {
        TLOG_MSG(("Black IP, stop"));
        return -1;
    }

    // 验票、验证签名是否来自 Controller
    spaTicketPacket.ParseFromString(msg);
    ret = SPATools().VerifyTicket(spaTicketPacket, RSA_PUB_KEY_CONTROLLER);
    iAssert(ret, ("DecryptVoucher faild"));
    MSG_PROTO(spaTicketPacket);

    // 票据ip 与 请求者ip 是否相同
    spa::SPATicket spaTicket = spaTicketPacket.ticket();
    std::string sign_data = spaTicketPacket.sign_data();
    if (spaTicket.ip() != from_ip)
    {
        TLOG_MSG(("Ticket Verify faild: IP mismatch"));
        return -1;
    }

    // 票据是否过期
    if (spaTicket.timestamp() + spaTicket.valid_seconds() < time(NULL))
    {
        TLOG_MSG(("Ticket Verify faild: Ticket expired"));
        return -1;
    }

    // 防止重放攻击
    ret = config->QueryAndInsertMD5(sign_data);
    iAssert(ret, ("Ticket Verify faild: MD5 repeat"));

    // 加入白名单 APPLICATION
    config->GetWhiteListObj()->OpWhiteList(IP_WHITE_LIST_ADD, from_ip, config->get_app_tcp_port());

    return 0;
}

// 此接口为Controller调用，控制黑名单
int SDPAppGatewayErpcServiceImpl::GateFuncBlackListOp(const erpc::GateFuncBlackListOpReq& objReq, erpc::GateFuncBlackListOpRsp& objRsp, const erpc::Extra& extra)
{
    MSG_PROTO(objReq);
    
    int ret = 0;
    int op = objReq.op();
    std::string from_ip = objReq.ip();

    auto config = SDPAppGatewayConfig::GetInstance();
    auto whitelist = config->GetWhiteListObj();
    int app_tcp_port = config->get_app_tcp_port();

    // 黑名单操作
    config->ControllerBlackList_OP(op, from_ip, app_tcp_port);

    // 添加黑名单时，删除白名单，需重新敲门，再作拦截
    // 去除黑名单后，不作拦截，重新敲门即可
    if (op == IP_BLACK_LIST_ADD)
    {
        ret = whitelist->OpWhiteList(IP_WHITE_LIST_DEL, from_ip, app_tcp_port);
        iAssert(ret, ("OpWhiteList faild"));
    }

    MSG_PROTO(objRsp);
    return 0;
}
