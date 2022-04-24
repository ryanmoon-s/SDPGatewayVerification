#pragma once

#include <map>
#include <string>
#include <vector>
#include "comm/erpc/erpc_config.h"
#include "sdp_application_service_impl.h"

class SDPApplicationErpcServiceImpl;

class SDPApplicationConfig {
public:
    SDPApplicationErpcServiceImpl* GetServiceObj();

private:
    SDPApplicationErpcServiceImpl* service_;

public:
    static SDPApplicationConfig* GetInstance() 
    {
        return instance_;
    }
    ~SDPApplicationConfig();

private:
    SDPApplicationConfig();
    SDPApplicationConfig(const SDPApplicationConfig&){};

    static SDPApplicationConfig* instance_;
};
