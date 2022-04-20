#include "sdp_controller_config.h"

SDPControllerConfig* SDPControllerConfig::instance_ = new SDPControllerConfig();

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
