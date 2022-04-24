#include "erpc_config.h"

ErpcConfig* ErpcConfig::instance_ = new ErpcConfig();

void ErpcConfig::SetServiceObj(ErpcService* service)
{
    service_ = service;
}

ErpcService* ErpcConfig::GetServiceObj()
{
    return service_;
}

void ErpcConfig::SetServerCert(const std::string& cert, const std::string& key)
{
    server_cert_ = cert;
    server_key_ = key;
}

std::string ErpcConfig::GetServerCert()
{
    return server_cert_;
}

std::string ErpcConfig::GetServerKey()
{
    return server_key_;
}