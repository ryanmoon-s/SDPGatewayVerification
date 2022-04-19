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

void ErpcConfig::SetWhiteListObj(ErpcIpWhiteList* whitelist)
{
    whitelist_ = whitelist;
}

ErpcIpWhiteList* ErpcConfig::GetWhiteListObj()
{
    return whitelist_;
}

bool ErpcIpWhiteList::IsIpInWhiteList(std::string ip)
{
    return false;
}
