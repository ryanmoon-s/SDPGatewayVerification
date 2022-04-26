#pragma once

#include "comm/erpc/erpc_service.h"
#include "comm/spatools/spa_tool.h"

class SDPApplicationErpcServiceImpl: public ErpcService {
public:

    virtual int AppFuncHttps(const std::string& request, std::string& response, const erpc::Extra& extra) override;

    int HTTPS_HANDLER(const std::string& request, std::string& response);
    
};
