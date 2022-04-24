#include "sdp_application.h"
#include "sdp_application_config.h"

SDPController::SDPController()
    : server_(ID_APPLICATION, IP_APPLICATION_IN, TCP_PORT_APPLICATION)
{
    auto config =  SDPApplicationConfig::GetInstance();
    auto service = config->GetServiceObj();

    // 注册服务
    server_.RegisterService(service, SSL_CRT_APPLICATION, SSL_KEY_APPLICATION);
}

void SDPController::Run()
{
    server_.Run();
}
