#pragma once

#include "comm/commdef/comm_def.h"
#include "comm/erpc/erpc_server.h"
#include "sdp_appgateway_service_impl.h"
#include "sdp_appgateway_whitelist_impl.h"

class SDPAppGateway {
public:
    void Run();

public:
    SDPAppGateway();



private:
    ErpcServer server_;
    SDPAppGatewayErpcServiceImpl service_;
    SDPAppGatewayErpcIpWhiteListImpl whitelist_;
};
