#include "sdp_appgateway_config.h"

SDPAppGatewayConfig* SDPAppGatewayConfig::instance_ = new SDPAppGatewayConfig();

SDPAppGatewayErpcIpWhiteListImpl* SDPAppGatewayConfig::GetWhiteListObj()
{
    return whitelist_;
}

SDPAppGatewayErpcServiceImpl* SDPAppGatewayConfig::GetServiceObj()
{
    return service_;
}

SDPAppGatewayConfig::SDPAppGatewayConfig()
{
    whitelist_ = new SDPAppGatewayErpcIpWhiteListImpl();
    service_ = new SDPAppGatewayErpcServiceImpl();
}

SDPAppGatewayConfig::~SDPAppGatewayConfig()
{
    delete whitelist_;
    delete service_;
}
