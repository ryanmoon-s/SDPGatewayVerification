#include "sdp_controller.h"

SDPController::SDPController(): server_(TCP_PORT_CONTROLLER, UDP_PORT_CONTROLLER)
{
    server_.RegisterService(&service_);
}

void SDPController::Run()
{
    server_.Run();
}