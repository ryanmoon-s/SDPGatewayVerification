#include "client.h"

int VerifyClient::GetAccessibleAppList(const spa::SPAVoucher& vocher)
{
    int ret = 0;
    spa::SPAVoucherPacket spaVoucherPacket;

    // SPA单包验证
    ret = SPATools().EncryptVoucher(spaVoucherPacket, vocher);
    iAssert(ret, ("EncryptVoucher faild"));

    ret = _SPAKnocking(spaVoucherPacket, IP_CONTROLLER_PB, UDP_PORT_CONTROLLER);
    iAssert(ret, ("SPAKnocking faild, ip:%s port:%d", IP_CONTROLLER_PB, UDP_PORT_CONTROLLER));
    TLOG_MSG(("SPAKnocking success, ip:%s, port:%d", IP_CONTROLLER_PB, UDP_PORT_CONTROLLER));

    // 获取可访问的应用列表
    

    
    return 0;
}

int VerifyClient::_SPAKnocking(const spa::SPAVoucherPacket& packet, std::string ip, int port) {
    int ret = 0;
    std::string msg;
    packet.SerializeToString(&msg);

    ret = ErpcClient().UDPFuncRequest(erpc::CMD_UDP_CONTROLLER_FUNC_RECV, msg, ip, port);
    iAssert(ret, ("TestFuncUdpRecv faild"));

    return 0;
}


int AccessClient::AccessApplication() {

}
