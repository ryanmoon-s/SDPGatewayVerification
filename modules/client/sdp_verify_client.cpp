#include "sdp_verify_client.h"
#include "comm/commdef/comm_tool.h"

int VerifyClient::GetAccessibleAppList(std::vector<erpc::AccessItem>& list, const spa::SPAVoucher& spaVoucher)
{
    int ret = 0;
    
    // SPA Controller
    ret = _SPAKnockingController(spaVoucher, IP_CONTROLLER_PB, UDP_PORT_CONTROLLER);
    iAssert(ret, ("SPAKnocking faild, ip:%s port:%d", IP_CONTROLLER_PB, UDP_PORT_CONTROLLER));

    // 获取可访问的应用列表
    erpc::ConFuncGetAccessReq req;
    erpc::ConFuncGetAccessRsp rsp;
    erpc::Header header;
    
    req.mutable_account()->CopyFrom(spaVoucher.account());  // 带上账号
    ret = erpc_client_.ConFuncGetAccessRequest(req, rsp, header);
    iAssert(ret, ("ConFuncGetAccessRequest faild"));

    // 处理可访问列表
    for (int i = 0; i < rsp.access_list_size(); i++)
    {
        erpc::AccessItem item = rsp.access_list(i);

        // 带上票据
        spa::SPATicketPacket spaTicketPacket;
        spaTicketPacket.CopyFrom(item.ticket_packet());
        DBG_PROTO(spaTicketPacket);
        
        // SPA 敲门 Gateway
        ret = _SPAKnockingGateway(spaTicketPacket, item.ip(), item.mutable_app()->gate_udp_port());
        iAssert(ret, ("_SPAKnockingGateway faild"));

        list.push_back(item);
    }

    // 输出可访问列表
    for (auto item : list)
    {
        std::string ip = item.ip();
        std::string appname = item.app().appname();
        std::string description = item.app().description();
        int app_tcp_port = item.app().app_tcp_port();
        int udp_port = item.app().gate_udp_port();

        TLOG_MSG(("Access ===== Get access list beg ====="));
        TLOG_MSG(("Access - ip:%s", ip.c_str()));
        TLOG_MSG(("Access - tcp_port:%d", app_tcp_port));
        TLOG_MSG(("Access - appname:%s", appname.c_str()));
        TLOG_MSG(("Access - description:%s", description.c_str()));
        TLOG_MSG(("Access ===== Get access list end ====="));
    }
    
    return 0;
}

int VerifyClient::_SPAKnockingController(const spa::SPAVoucher& spaVoucher, std::string ip, int port) {
    int ret = 0;
    std::string msg;
    spa::SPAVoucherPacket spaVoucherPacket;
    
    ret = SPATools().EncryptVoucher(spaVoucherPacket, spaVoucher, RSA_PUB_KEY_CONTROLLER);
    iAssert(ret, ("EncryptVoucher faild"));

    spaVoucherPacket.SerializeToString(&msg);
    ret = erpc_client_.UDPFuncRequest(erpc::CMD_UDP_CONTROLLER_FUNC_RECV, msg, ip, port);
    iAssert(ret, ("TestFuncUdpRecv faild"));

    TLOG_MSG(("SPA Knocking controller success, ip:%s, port:%d", ip.c_str(), port));
    return 0;
}

int VerifyClient::_SPAKnockingGateway(const spa::SPATicketPacket &spaTicketPacket, std::string ip, int port)
{
    int ret = 0;
    std::string msg;

    spaTicketPacket.SerializeToString(&msg);
    ret = erpc_client_.UDPFuncRequest(erpc::CMD_UDP_APPGATEWAY_FUNC_RECV, msg, ip, port);
    iAssert(ret, ("UDPFuncRequest faild"));

    TLOG_MSG(("SPA Knocking gateway success, ip:%s, port:%d", ip.c_str(), port));
    return 0;
}