#include "erpc_config.h"

ErpcConfig* ErpcConfig::instance_ = new ErpcConfig();

// op: IP_WHITE_TABLE_OP
void ErpcConfig::IpWhiteTableOp(int op, std::string ip)
{
    if (op == IP_WHITE_TABLE_ADD)
    {
        ip_white_table_[ip] = 1;
    }
    else if (op == IP_WHITE_TABLE_DEL)
    {
        ip_white_table_.erase(ip);
    }
}

bool ErpcConfig::IsIpInWhiteTable(std::string ip)
{
    auto iter = ip_white_table_.find(ip);
    return iter != ip_white_table_.end();
}
