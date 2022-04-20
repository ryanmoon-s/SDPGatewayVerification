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
