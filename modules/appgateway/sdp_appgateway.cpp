#include "sdp_appgateway.h"

SDPAppGateway::SDPAppGateway(): server_(TCP_PORT_APPGATEWAY, UDP_PORT_APPGATEWAY)
{
    auto whitelist = SDPAppGatewayConfig::GetInstance()->GetWhiteListObj();
    auto service = SDPAppGatewayConfig::GetInstance()->GetServiceObj();
    
    whitelist->OpIpWhiteList(IP_WHITE_TABLE_ADD, "127.0.0.1");
    
    server_.RegisterService(service);
    server_.RegisterWhiteList(whitelist);
}

void SDPAppGateway::Run()
{
    server_.Run();
}
