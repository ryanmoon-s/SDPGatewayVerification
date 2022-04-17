#include "client.h"

int VerifyClient::GetAccessibleAppList(vector<pair<std::string, int>>& list, const spa::SPAVoucher& vocher)
{
    int ret = 0;
    spa::SPAPacket packet;

    // SPA单包验证
    ret = SPATools().EncryptVoucher(packet, vocher);
    iAssert(ret, ("EncryptVoucher packet content:%s", packet.encrypt_spa_voucher().c_str()));

    ret = _SPAKnocking(packet, IP_CONTROLLER, UDP_PORT_CONTROLLER);
    iAssert(ret, ("SPAKnocking ip:%s port:%d", IP_CONTROLLER, UDP_PORT_CONTROLLER));

    // 获取可访问的应用列表
    

    
    return 0;
}

int VerifyClient::_SPAKnocking(const spa::SPAPacket& packet, std::string ip, int port) {
    int ret = 0, fd;
    struct sockaddr_in addr;
    std::string data = packet.encrypt_spa_voucher();

    fd = socket(AF_INET, SOCK_DGRAM, 0); 
    iAssert(fd, ("socket faild"));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    // ret = sendto(fd, data.c_str(), data.size(), (struct sockaddr*)&addr, sizeof(addr));
    ErpcHandler().UDPSend("lalala ... ...", ip, port);
    iAssert(ret, ("sendto ret:%d, ip:%s port:%d", ret, ip.c_str(), port));

    return 0;
}


int AccessClient::AccessApplication() {

}
