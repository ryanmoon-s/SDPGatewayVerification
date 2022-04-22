#include "client.h"
#include "comm/erpc/erpc_client.h"

int VerifyClient::GetAccessibleAppList(const spa::SPAVoucher& spaVoucher)
{
    int ret = 0;
    
    // SPA单包认证
    ret = _SPAKnockingController(spaVoucher, IP_CONTROLLER_PB, UDP_PORT_CONTROLLER);
    iAssert(ret, ("SPAKnocking faild, ip:%s port:%d", IP_CONTROLLER_PB, UDP_PORT_CONTROLLER));

    // 获取可访问的应用列表
    erpc::ConFuncGetAccessReq req;
    erpc::ConFuncGetAccessRsp rsp;
    erpc::Header header;
    ret = ErpcClient().ConFuncGetAccessRequest(req, rsp, header);
    iAssert(ret, ("ConFuncGetAccessRequest faild"));

    // 票据、Access List
    
    
    return 0;
}

int VerifyClient::_SPAKnockingController(const spa::SPAVoucher& spaVoucher, std::string ip, int port) {
    int ret = 0;
    std::string msg;
    spa::SPAVoucherPacket spaVoucherPacket;
    
    ret = SPATools().EncryptVoucher(spaVoucherPacket, spaVoucher);
    iAssert(ret, ("EncryptVoucher faild"));

    spaVoucherPacket.SerializeToString(&msg);
    ret = ErpcClient().UDPFuncRequest(erpc::CMD_UDP_CONTROLLER_FUNC_RECV, msg, ip, port);
    iAssert(ret, ("TestFuncUdpRecv faild"));

    TLOG_MSG(("SPAKnocking success, ip:%s, port:%d", ip.c_str(), port));
    return 0;
}


int AccessClient::AccessApplication() {

}
