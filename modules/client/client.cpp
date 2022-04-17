#include "client.h"

int VerifyClient::GetAccessibleAppList(vector<pair<std::string, int>>& list, const spa::SPAVoucher& vocher)
{
    // SPA单包验证
    spa::SPAPacket packet;
    int ret = SPATools().EncryptVoucher(packet, vocher);
    iAssert(ret, ("EncryptVoucher packet content:%s", packet.encrypt_spa_voucher().c_str()));

    ret = _SPAKnocking(packet, IP_CONTROLLER, UDP_PORT_CONTROLLER);
    iAssert(ret, ("SPAKnocking ip:%s port:%d", IP_CONTROLLER, UDP_PORT_CONTROLLER));

    // 获取可访问的应用列表
    

    
    return 0;
}

int VerifyClient::_SPAKnocking(const spa::SPAPacket& packet, std::string ip, int port) {
    std::string data = packet.encrypt_spa_voucher();
    int ret = 0, fd;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_DGRAM, 0); 
    iAssert(fd, ("socket"));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    ret = sendto(fd, data.c_str(), data.size(), (struct sockaddr*)&addr, sizeof(addr));
    iAssert(ret, ("connect ip:%s port:%d", ip.c_str(), port));

    return 0;
}


int AccessClient::AccessApplication() {

}
