#pragma once

#include "comm/commdef/comm_def.h"
#include "comm/erpc/erpc_server.h"
#include "sdp_controller_service_impl.h"
#include "sdp_controller_whitelist_impl.h"

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
