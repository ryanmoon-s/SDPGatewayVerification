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

void SDPAppGatewayConfig::set_listen_info(const std::string& ip, int tcp_port, int udp_port, int app_tcp_port)
{
    listen_ip_ = ip;
    tcp_port_ = tcp_port;
    udp_port_ = udp_port;
    app_tcp_port_ = app_tcp_port;
}

int SDPAppGatewayConfig::get_tcp_port()
{
    return tcp_port_;
}

int SDPAppGatewayConfig::get_app_tcp_port()
{
    return app_tcp_port_;
}

int SDPAppGatewayConfig::QueryAndInsertMD5(const std::string& md5)
{
    auto iter = md5_map_.find(md5);
    if (iter != md5_map_.end())
    {
        TLOG_ERR(("md5 repeated"));
        return -1;
    }
    else
    {
        md5_map_.insert(std::make_pair(md5, 1));
        return 0;
    }
    return 0;
}

int SDPAppGatewayConfig::ControllerBlackList_OP(int op, const std::string& ip, int port)
{
    if (op == IP_BLACK_LIST_ADD)
    {
        controller_black_list_.insert(std::make_pair(ip, port));
    }
    else if (op == IP_BLACK_LIST_DEL)
    {
        controller_black_list_.erase(ip);
    }

    TLOG_DBG(("success, op:%d, ip:%s, port:%d", op, ip.c_str(), port));
    return 0;
}

bool SDPAppGatewayConfig::ControllerBlackList_IsIn(std::string ip, int port)
{
    auto iter = controller_black_list_.find(ip);
    if (iter != controller_black_list_.end() && iter->second == port)
    {
        return true;
    }
    return false;
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
