#include "sdp_gateway_service_impl.h"
#include "comm/commdef/comm_tool.h"

using namespace commtool;

int SDPAppGatewayErpcServiceImpl::GateFuncUdpRecv(const std::string& msg, std::string ip, int port)
{
    int ret = 0;
    spa::SPATicketPacket spaTicketPacket;
    spa::SPATicket spaTicket;

    // 解出 spaVoucher
    spaTicketPacket.ParseFromString(msg);
    ret = SPATools().DecryptVoucher(spaTicket, spaTicketPacket);
    iAssert(ret, ("DecryptVoucher faild"));
    TLOG_PROTO(spaTicket);

    //


    return 0;
}

int SDPAppGatewayErpcServiceImpl::GateFuncWhiteListOp(const erpc::GateFuncWhiteListOpReq& objReq, erpc::GateFuncWhiteListOpRsp& objRsp, const erpc::Extra& extra)
{
    TLOG_PROTO(objReq);
    
    auto config = SDPAppGatewayConfig::GetInstance();
    auto whitelist = config->GetWhiteListObj();
    whitelist->OpWhiteList(objReq.op(), objReq.ip(), config->get_tcp_port());

    TLOG_PROTO(objReq);
    return 0;
}
