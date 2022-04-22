#include "comm_tool.h"
#include "comm/tlog/tlog.h"

int commtool::GetMacAddress(std::string& mac_address)
{
    char mac[32];
    struct ifreq ifreq;
    int sock, ret = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    iAssert(sock, ("socket"));

    strcpy(ifreq.ifr_name, "eth0");

    ret = ioctl(sock, SIOCGIFHWADDR, &ifreq);
    iAssert(ret, ("ioctl"));

    ret = snprintf(mac, sizeof(mac), "%02X:%02X:%02X:%02X:%02X:%02X", 
        (unsigned char) ifreq.ifr_hwaddr.sa_data[0],
        (unsigned char) ifreq.ifr_hwaddr.sa_data[1],
        (unsigned char) ifreq.ifr_hwaddr.sa_data[2],
        (unsigned char) ifreq.ifr_hwaddr.sa_data[3],
        (unsigned char) ifreq.ifr_hwaddr.sa_data[4],
        (unsigned char) ifreq.ifr_hwaddr.sa_data[5]);

    mac_address.assign(mac, ret);

    return 0;
}
