#include "comm_tool.h"
#include "comm/tlog/tlog.h"


std::string commtool::GetMacAddress()
{
    char mac[32];
    struct ifreq ifreq;
    int sock, ret = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        TLOG_ERR(("socket"));
        return "";
    }

    strcpy(ifreq.ifr_name, "eth0");

    ret = ioctl(sock, SIOCGIFHWADDR, &ifreq);
    if (ret < 0)
    {
        TLOG_ERR(("socket"));
        return "";
    }

    ret = snprintf(mac, sizeof(mac), "%02X:%02X:%02X:%02X:%02X:%02X", 
        (unsigned char) ifreq.ifr_hwaddr.sa_data[0],
        (unsigned char) ifreq.ifr_hwaddr.sa_data[1],
        (unsigned char) ifreq.ifr_hwaddr.sa_data[2],
        (unsigned char) ifreq.ifr_hwaddr.sa_data[3],
        (unsigned char) ifreq.ifr_hwaddr.sa_data[4],
        (unsigned char) ifreq.ifr_hwaddr.sa_data[5]);

    return std::string(mac, ret);
}

std::string commtool::Proto2Json(const google::protobuf::Message& proto) {
    std::string json;
    google::protobuf::util::MessageToJsonString(proto, &json);
    return json;
}


time_t commtool::TimeNow_Min()
{
    time_t seconds = time(NULL);
    struct tm * tm = localtime(&seconds);
    tm->tm_sec = 0;
    return  mktime(tm);
}
