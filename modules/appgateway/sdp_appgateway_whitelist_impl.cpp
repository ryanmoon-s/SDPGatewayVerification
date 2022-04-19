#include "sdp_appgateway_whitelist_impl.h"

// op: IP_WHITE_TABLE_OP
void SDPAppGatewayErpcIpWhiteListImpl::OpIpWhiteList(int op, std::string ip)
{
    if (op == IP_WHITE_TABLE_ADD)
    {
        ip_white_list_.insert(std::make_pair(ip, 1));
    }
    else if (op == IP_WHITE_TABLE_DEL)
    {
        ip_white_list_.erase(ip);
    }
    TLOG_MSG(("OpIpWhiteList, op:%d, ip:%s", op, ip.c_str()));
}

bool SDPAppGatewayErpcIpWhiteListImpl::IsIpInWhiteList(std::string ip)
{
    auto iter = ip_white_list_.find(ip);
    return iter != ip_white_list_.end();
}
