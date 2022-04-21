#include "sdp_gateway_config.h"

SDPAppGatewayConfig* SDPAppGatewayConfig::instance_ = new SDPAppGatewayConfig();

IPWhiteList* SDPAppGatewayConfig::GetWhiteListObj()
{
    return whitelist_;
}

SDPAppGatewayErpcServiceImpl* SDPAppGatewayConfig::GetServiceObj()
{
    return service_;
}

void SDPAppGatewayConfig::set_listen_info(const std::string& ip, int tcp_port, int udp_port)
{
    listen_ip_ = ip;
    tcp_port_ = tcp_port;
    udp_port_ = udp_port;
}

int SDPAppGatewayConfig::get_tcp_port()
{
    return tcp_port_;
}

SDPAppGatewayConfig::SDPAppGatewayConfig()
{
    whitelist_ = new IPWhiteList();
    service_ = new SDPAppGatewayErpcServiceImpl();
}

SDPAppGatewayConfig::~SDPAppGatewayConfig()
{
    delete whitelist_;
    delete service_;
}
