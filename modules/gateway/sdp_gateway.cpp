#include "sdp_gateway.h"
#include "comm/iptools/iptables_tool.h"
#include "comm/erpc/erpc_client.h"
#include <vector>

SDPAppGateway::SDPAppGateway()
        : server_(ID_GATEWAY, IP_APPGATEWAY_IN, TCP_PORT_APPGATEWAY, UDP_PORT_APPGATEWAY)
{
    int ret = 0;
    auto config =  SDPAppGatewayConfig::GetInstance();
    auto whitelist = config->GetWhiteListObj();
    auto service = config->GetServiceObj();

    // 配置 config
    config->set_listen_info(IP_APPGATEWAY_IN, TCP_PORT_APPGATEWAY, UDP_PORT_APPGATEWAY);

    // 初始化防火墙
    std::vector<std::string> white_vec;
    white_vec.push_back(IP_CONTROLLER_PB);
    whitelist->InitWhiteList(white_vec, TCP_PORT_APPGATEWAY);

    // 注册服务
    server_.RegisterService(service, SSL_CRT_GATEWAY, SSL_KEY_GATEWAY);

    // 向 Controller 注册应用
    erpc::ConFuncRegisterAppReq req;
    erpc::ConFuncRegisterAppRsp rsp;
    erpc::Header header;

    // TODO添加更多可扩展的应用 基于id管理
    erpc::AppItem* item = req.add_app_list();
    item->set_udp_port(UDP_PORT_APPGATEWAY);
    item->set_tcp_port(TCP_PORT_APPLICATION);
    item->set_appname("HTTPS");
    item->set_description("Http over tls");

    ret = ErpcClient(SSL_CRT_GATEWAY, SSL_KEY_GATEWAY).ConFuncRegisterAppRequest(req, rsp, header);
    iAssertNoReturn(ret, ("ConFuncRegisterAppRequest faild"));
}

void SDPAppGateway::Run()
{
    server_.Run();
}
