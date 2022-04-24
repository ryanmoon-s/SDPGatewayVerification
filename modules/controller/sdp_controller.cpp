#include "sdp_controller.h"
#include "sdp_controller_config.h"

#include "comm/iptools/iptables_tool.h"
#include <vector>

SDPController::SDPController()
    : server_(ID_CONTROLLER, IP_CONTROLLER_IN, TCP_PORT_CONTROLLER, UDP_PORT_CONTROLLER)
{
    auto config =  SDPControllerConfig::GetInstance();
    auto whitelist = config->GetWhiteListObj();
    auto service = config->GetServiceObj();

    // 配置config
    config->set_listen_info(IP_CONTROLLER_IN, TCP_PORT_CONTROLLER, UDP_PORT_CONTROLLER);

    // 初始化防火墙
    std::vector<std::string> white_vec;
    white_vec.push_back(IP_APPGATEWAY_PB);
    whitelist->InitWhiteList(white_vec, TCP_PORT_CONTROLLER);
    
    // 注册服务
    server_.RegisterService(service, SSL_CRT_CONTROLLER, SSL_KEY_CONTROLLER);

    // 初始化随机数种子
    srand(time(NULL));
}

void SDPController::Run()
{
    server_.Run();
}
