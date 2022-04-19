#pragma once

#include "comm/commdef/comm_def.h"
#include "comm/erpc/erpc_server.h"
#include "sdp_controller_erpc_serviceimpl.h"
#include "sdp_controller_erpc_ipwhitelistimpl.h"

class SDPController {
public:
    void Run();

public:
    SDPController();



private:
    ErpcServer server_;
    SDPControllerErpcServiceImpl service_;
    SDPControllerErpcIpWhiteListImpl whitelist_;
};
