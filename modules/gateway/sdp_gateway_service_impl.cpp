#include "sdp_gateway_service_impl.h"
#include "comm/commdef/comm_tool.h"

using namespace commtool;

int SDPAppGatewayErpcServiceImpl::GateFuncUdpRecv(const std::string& msg, std::string ip, int port)
{
    TLOG_MSG(("GateFuncUdpRecv begin size:%d", msg.size()));
    int ret = 0;
    spa::SPATicketPacket spaTicketPacket;
    auto config = SDPAppGatewayConfig::GetInstance();

    // 验证签名是否来自 Controller
    spaTicketPacket.ParseFromString(msg);
    ret = SPATools().VerifyTicket(spaTicketPacket, RSA_PUB_KEY_CONTROLLER);
    iAssert(ret, ("DecryptVoucher faild"));
    MSG_PROTO(spaTicketPacket);

    // 票据ip 与 请求者ip 是否相同
    std::string tick_ip = spaTicketPacket.ticket().ip();
    std::string sign_data = spaTicketPacket.sign_data();
    if (tick_ip != ip)
    {
        TLOG_MSG(("Verify faild: IP mismatch"));
        return -1;
    }

    // 防止重放攻击
    ret = config->QueryAndInsertMD5(sign_data);
    if (ret != 0)
    {
        TLOG_MSG(("Verify faild: MD5 repeat"));
        return -1;
    }

    // 加入白名单
    config->GetWhiteListObj()->OpWhiteList(IP_WHITE_LIST_ADD, ip, port);

    return 0;
}

int SDPAppGatewayErpcServiceImpl::GateFuncWhiteListOp(const erpc::GateFuncWhiteListOpReq& objReq, erpc::GateFuncWhiteListOpRsp& objRsp, const erpc::Extra& extra)
{
    MSG_PROTO(objReq);
    
    auto config = SDPAppGatewayConfig::GetInstance();
    auto whitelist = config->GetWhiteListObj();
    whitelist->OpWhiteList(objReq.op(), objReq.ip(), config->get_tcp_port());

    MSG_PROTO(objRsp);
    return 0;
}
