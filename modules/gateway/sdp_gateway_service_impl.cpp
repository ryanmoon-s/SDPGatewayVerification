#include "sdp_gateway_service_impl.h"
#include "comm/commdef/comm_tool.h"

using namespace commtool;

int SDPAppGatewayErpcServiceImpl::GateFuncUdpRecv(const std::string& msg, std::string from_ip, int from_port)
{
    TLOG_MSG(("GateFuncUdpRecv begin size:%d", msg.size()));
    int ret = 0;
    spa::SPATicket spaTicket;
    spa::SPATicketPacket spaTicketPacket;
    auto config = SDPAppGatewayConfig::GetInstance();

    // 拦截Controller下发黑名单
    bool isblack = config->ControllerBlackList_IsIn(from_ip, config->get_app_tcp_port());
    if (isblack)
    {
        TLOG_WARN((" ~~~ BLACK IP, STOP VERIFY ~~~ "));
        return 1;
    }
    TLOG_MSG(("VERIFY: not black"));

    // 验票、验证签名是否来自 Controller
    spaTicketPacket.ParseFromString(msg);
    ret = SPATools().DecryptTicket(spaTicket, spaTicketPacket, RSA_PUB_KEY_CONTROLLER, RSA_PRI_KEY_GATEWAY);
    iAssert(ret, ("DecryptVoucher faild"));
    MSG_PROTO(spaTicketPacket);

    // 票据ip 与 请求者ip 是否相同
    std::string sign_data = spaTicketPacket.sign_data();
    if (spaTicket.ip() != from_ip)
    {
        TLOG_WARN((" ~~~ IP DISMATCH, STOP VERIFY ~~~ "));
        return 1;
    }
    TLOG_MSG(("VERIFY: ip matched"));

    // 票据是否过期
    if (spaTicket.timestamp() + spaTicket.valid_seconds() < time(NULL))
    {
        TLOG_WARN((" ~~~ TICKET EXPIRED, STOP VERIFY ~~~ "));
        return 1;
    }
    TLOG_MSG(("VERIFY: ticket time valid"));

    // 防止重放攻击
    ret = config->QueryAndInsertMD5(sign_data);
    if (ret != 0)
    {
        TLOG_WARN((" ~~~ MD5 REPEATED, STOP VERIFY ~~~ "));
        return 1;
    }
    TLOG_MSG(("VERIFY: not repeated md5"));

    // 加入白名单 APPLICATION
    config->GetWhiteListObj()->OpWhiteList(IP_WHITE_LIST_ADD, from_ip, config->get_app_tcp_port());

    TLOG_MSG((" ~~ NOTICE: GATEWAY SPA VERIFICATION PASSED ~~ "));

    return 0;
}

// 此接口为Controller调用，控制黑名单
// 添加后，需要Controller主动触发去除
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

    TLOG_MSG((" ************************* %s BLACK ************************* "
            , op == IP_BLACK_LIST_ADD ? "ADD" : "DEL"));

    MSG_PROTO(objRsp);
    return 0;
}

int SDPAppGatewayErpcServiceImpl::GateFuncNotice(const erpc::GateFuncNoticeReq& objReq, erpc::GateFuncNoticeRsp& objRsp, const erpc::Extra& extra)
{
    MSG_PROTO(objReq);

    int ret = 0;
    auto config = SDPAppGatewayConfig::GetInstance();
    auto whitelist = config->GetWhiteListObj();
    int app_tcp_port = config->get_app_tcp_port();

    // 鉴权，权APPLICATION可调用此接口
    std::string from_ip = extra.socket_info.ip;
    if (from_ip != IP_APPLICATION_PB)
    {
        TLOG_WARN(("check permission faild, only application can all"));
        return 1;
    }

    // 应用通知去白
    if (objReq.op() == erpc::APP_NOTICE_CLOSE_PORT_FOR_IP)
    {
        ret = whitelist->OpWhiteList(IP_WHITE_LIST_DEL, objReq.op_ip(), app_tcp_port);
        iAssert(ret, ("OpWhiteList faild"));
    }

    TLOG_MSG((" ************************* APP NOTICE DEL BLACK ************************* "));

    MSG_PROTO(objRsp);
}
