#pragma once

#include <map>
#include <string>
#include "comm/erpc/erpc_config.h"
#include "comm/iptools/iptables_tool.h"
#include "sdp_controller_service_impl.h"

class SDPControllerErpcServiceImpl;

class SDPControllerConfig {
public:
    IPWhiteList* GetWhiteListObj();
    SDPControllerErpcServiceImpl* GetServiceObj();

    void set_listen_info(const std::string& ip, int tcp_port, int udp_port);
    int get_tcp_port();

    int QueryAndInsertMD5(const std::string& md5);

private:
    IPWhiteList* whitelist_;
    SDPControllerErpcServiceImpl* service_;

    std::string listen_ip_;
    int tcp_port_;
    int udp_port_;

    // md5去重，防重放攻击
    std::map<std::string, int> md5_map_;

public:
    static SDPControllerConfig* GetInstance() 
    {
        return instance_;
    }
    ~SDPControllerConfig();

private:
    SDPControllerConfig();
    SDPControllerConfig(const SDPControllerConfig&){};

    static SDPControllerConfig* instance_;
};
