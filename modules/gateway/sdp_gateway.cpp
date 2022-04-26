#include "sdp_gateway.h"
#include "comm/iptools/iptables_tool.h"
#include "comm/erpc/erpc_client.h"
#include <vector>

SDPAppGateway::SDPAppGateway()
        : server_(erpc::ID_GATEWAY, IP_APPGATEWAY_IN, TCP_PORT_APPGATEWAY, UDP_PORT_APPGATEWAY)
{
    int ret = 0;
    auto config =  SDPAppGatewayConfig::GetInstance();
    auto whitelist = config->GetWhiteListObj();
    auto service = config->GetServiceObj();

    // 配置 config
    config->set_listen_info(IP_APPGATEWAY_IN, TCP_PORT_APPGATEWAY, UDP_PORT_APPGATEWAY, TCP_PORT_APPLICATION);

    // 初始化防火墙; 与Controller交互端口; 加入CONTROLLER
    std::vector<std::string> white_vec;
    white_vec.push_back(IP_CONTROLLER_PB); 
    whitelist->InitWhiteList(white_vec, config->get_tcp_port());

    // 初始化防火墙; 应用通信端口
    white_vec.clear();
    whitelist->InitWhiteList(white_vec, config->get_app_tcp_port());

    // 注册RPC服务
    server_.RegisterService(service, SSL_CRT_GATEWAY, SSL_KEY_GATEWAY);

    // 向 Controller 注册应用
    erpc::ConFuncRegisterAppReq req;
    erpc::ConFuncRegisterAppRsp rsp;
    erpc::Header header;
    // TODO 添加更多可扩展的应用 基于id管理
    erpc::AppItem* item = req.add_app_list();
    item->set_gate_udp_port(UDP_PORT_APPGATEWAY);
    item->set_app_tcp_port(TCP_PORT_APPLICATION);
    item->set_appname("HTTPS");
    item->set_description("Http over tls");
    ret = ErpcClient(SSL_CRT_GATEWAY, SSL_KEY_GATEWAY).ConFuncRegisterAppRequest(req, rsp, header);
    iAssertNoReturn(ret, ("ConFuncRegisterAppRequest faild"));
}

void SDPAppGateway::Run()
{
    server_.Run();
}
