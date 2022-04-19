#pragma once

#include <map>
#include <string>
#include "comm/erpc/erpc_config.h"
#include "sdp_appgateway_whitelist_impl.h"
#include "sdp_appgateway_service_impl.h"

class SDPAppGatewayErpcServiceImpl;

class SDPAppGatewayConfig {
public:
    SDPAppGatewayErpcIpWhiteListImpl* GetWhiteListObj();
    SDPAppGatewayErpcServiceImpl* GetServiceObj();


private:
    SDPAppGatewayErpcIpWhiteListImpl* whitelist_;
    SDPAppGatewayErpcServiceImpl* service_;

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
