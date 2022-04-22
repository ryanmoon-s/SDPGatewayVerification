#pragma once

#include <map>
#include <string>
#include "erpc_service.h"

class ErpcService;

class ErpcConfig {
public:
    // service
    void SetServiceObj(ErpcService* service);
    ErpcService* GetServiceObj();

private:
    ErpcService* service_;

// 单例
public:
    static ErpcConfig* GetInstance() 
    {
        return instance_;
    }

private:
    ErpcConfig(){};
    ErpcConfig(const ErpcConfig&){};

    static ErpcConfig* instance_;
};
