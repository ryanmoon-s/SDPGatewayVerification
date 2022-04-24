#pragma once

#include "comm/commdef/comm_def.h"
#include "comm/erpc/erpc_server.h"
#include "sdp_application_service_impl.h"

class SDPApplication {
public:
    void Run();

public:
    SDPApplication();



private:
    ErpcServer server_;
};
