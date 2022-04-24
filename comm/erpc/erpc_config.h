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

    // cert
    void SetServerCert(const std::string& cert, const std::string& key);
    std::string GetServerCert();
    std::string GetServerKey();

private:
    ErpcService* service_;
    std::string server_cert_;
    std::string server_key_;
    
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
