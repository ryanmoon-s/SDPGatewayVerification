#include "controller.h"

SDPController::SDPController(): server_(TCP_PORT_CONTROLLER, UDP_PORT_CONTROLLER)
{
    // whitelist_.OpIpWhiteList(IP_WHITE_TABLE_ADD, "127.0.0.1");

    server_.RegisterService(&service_);
    server_.RegisterWhiteList(&whitelist_);
}

void SDPController::Run()
{
    server_.Run();
}
