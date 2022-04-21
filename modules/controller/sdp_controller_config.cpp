#include "sdp_controller_config.h"

SDPControllerConfig* SDPControllerConfig::instance_ = new SDPControllerConfig();

SDPControllerConfig::SDPControllerConfig()
{
    whitelist_ = new IPWhiteList();
    service_ = new SDPControllerErpcServiceImpl();
}

SDPControllerConfig::~SDPControllerConfig()
{
    delete whitelist_;
    delete service_;
}

IPWhiteList* SDPControllerConfig::GetWhiteListObj()
{
    return whitelist_;
}

SDPControllerErpcServiceImpl* SDPControllerConfig::GetServiceObj()
{
    return service_;
}

void SDPControllerConfig::set_listen_info(const std::string& ip, int tcp_port, int udp_port)
{
    listen_ip_ = ip;
    tcp_port_ = tcp_port;
    udp_port_ = udp_port;
}

int SDPControllerConfig::get_tcp_port()
{
    return tcp_port_;
}

int SDPControllerConfig::QueryAndInsertMD5(const std::string& md5)
{
    auto iter = md5_map_.find(md5);
    if (iter != md5_map_.end())
    {
        // 找到
        return 1;
    }
    else
    {
        // 没找到
        md5_map_.insert(std::make_pair(md5, 1));
        return 0;
    }
    return 0;
}

