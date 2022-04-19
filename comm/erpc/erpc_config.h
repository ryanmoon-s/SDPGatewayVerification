#pragma once

#include <map>
#include <string>
#include "erpc_service.h"

class ErpcIpWhiteList;

class ErpcConfig {
public:
    // service
    void SetServiceObj(ErpcService* service);
    ErpcService* GetServiceObj();

    // white list
    void SetWhiteListObj(ErpcIpWhiteList* whitelist);
    ErpcIpWhiteList* GetWhiteListObj();

public:


private:
    ErpcService* service_;
    ErpcIpWhiteList* whitelist_;

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

class ErpcIpWhiteList {
public:
    virtual bool IsIpInWhiteList(std::string ip);
};
