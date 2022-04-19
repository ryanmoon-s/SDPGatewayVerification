#include "controller_erpc_ipwhitelistimpl.h"

// op: IP_WHITE_TABLE_OP
void SDPControllerErpcIpWhiteListImpl::OpIpWhiteList(int op, std::string ip)
{
    if (op == IP_WHITE_TABLE_ADD)
    {
        ip_white_list_.insert(std::make_pair(ip, 1));
    }
    else if (op == IP_WHITE_TABLE_DEL)
    {
        ip_white_list_.erase(ip);
    }
}

bool SDPControllerErpcIpWhiteListImpl::IsIpInWhiteList(std::string ip)
{
    auto iter = ip_white_list_.find(ip);
    return iter != ip_white_list_.end();
}
