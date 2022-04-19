#include "sdp_appgateway.h"

SDPAppGateway::SDPAppGateway(): server_(TCP_PORT_APPGATEWAY, UDP_PORT_APPGATEWAY)
{
    whitelist_.OpIpWhiteList(IP_WHITE_TABLE_ADD, "127.0.0.1");

    server_.RegisterService(&service_);
    server_.RegisterWhiteList(&whitelist_);
}

void SDPAppGateway::Run()
{
    server_.Run();
}
