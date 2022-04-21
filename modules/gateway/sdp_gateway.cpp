#include "sdp_gateway.h"
#include "comm/iptools/iptables_tool.h"
#include <vector>

SDPAppGateway::SDPAppGateway(): server_(IP_APPGATEWAY_IN, TCP_PORT_APPGATEWAY, UDP_PORT_APPGATEWAY)
{
    auto config =  SDPAppGatewayConfig::GetInstance();
    auto whitelist = config->GetWhiteListObj();
    auto service = config->GetServiceObj();

    // 配置config
    config->set_listen_info(IP_APPGATEWAY_IN, TCP_PORT_APPGATEWAY, UDP_PORT_APPGATEWAY);

    // 初始化防火墙
    std::vector<std::string> white_vec;
    white_vec.push_back(IP_CONTROLLER_PB);
    whitelist->InitWhiteList(white_vec, TCP_PORT_APPGATEWAY);
    
    // 注册服务
    server_.RegisterService(service);
}

void SDPAppGateway::Run()
{
    server_.Run();
}
