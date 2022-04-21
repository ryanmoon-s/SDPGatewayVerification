#pragma once

#include <map>
#include <string>
#include "comm/erpc/erpc_config.h"
#include "comm/iptools/iptables_tool.h"
#include "sdp_gateway_service_impl.h"

class SDPAppGatewayErpcServiceImpl;

class SDPAppGatewayConfig {
public:
    IPWhiteList* GetWhiteListObj();
    SDPAppGatewayErpcServiceImpl* GetServiceObj();

    void set_listen_info(const std::string& ip, int tcp_port, int udp_port);
    int get_tcp_port();

private:
    IPWhiteList* whitelist_;
    SDPAppGatewayErpcServiceImpl* service_;

    std::string listen_ip_;
    int tcp_port_;
    int udp_port_;

// 单例
public:
    static SDPAppGatewayConfig* GetInstance() 
    {
        return instance_;
    }
    ~SDPAppGatewayConfig();

private:
    SDPAppGatewayConfig();
    SDPAppGatewayConfig(const SDPAppGatewayConfig&){};

    static SDPAppGatewayConfig* instance_;
};
