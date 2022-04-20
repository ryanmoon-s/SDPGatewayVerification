#include "iptables_tool.h"
#include "comm/tlog/tlog.h"

// 暂时不调用system，后面放开 TODO
// #define SYSTEM_EXEC_CMD ret = system(cmd.c_str())
#define SYSTEM_EXEC_CMD {}

int IptablesTools::IptablesInit(const std::vector<std::string>& whitelist, int port)
{
    int ret = 0;

    // ban all
    ret = _BanAll(port);
    if (ret < 0)
    {
        return -1;
    }

    // accept white list
    for (int i = 0; i < whitelist.size(); i++)
    {
        ret = UnBanIp(whitelist[i], port);
        if (ret < 0)
        {
            return -1;
        }
    }

    return 0;
}

int IptablesTools::_BanAll(int port)
{
    int ret = 0;
    std::string s_port = std::to_string(port);

    std::string cmd = "iptables -I INPUT -p tcp --dport " + s_port + " -j DROP";
    SYSTEM_EXEC_CMD
    TLOG_DBG(("_BanAll "));

    return (ret == 0 ? 0 : -1);
}

int IptablesTools::BanIp(const std::string& ip, int port)
{
    int ret = 0;
    std::string s_port = std::to_string(port);

    std::string cmd = "iptables -D INPUT -s " + ip + " -p tcp --dport " + s_port + " -j ACCEPT";
    SYSTEM_EXEC_CMD
    TLOG_DBG(("BanIp exec"));

    return (ret == 0 ? 0 : -1);
}

int IptablesTools::UnBanIp(const std::string& ip, int port)
{
    int ret = 0;

    std::string cmd = "iptables -I INPUT -s " + ip + " -p tcp --dport " + std::to_string(port) + " -j ACCEPT";
    SYSTEM_EXEC_CMD
    TLOG_DBG(("UnBanIp exec"));

    return (ret == 0 ? 0 : -1);
}

// op: IP_WHITE_TABLE_OP
int IPWhiteList::OpWhiteList(int op, std::string ip, int port)
{
    IptablesTools iptool;
    int ret = 0;

    if (op == IP_WHITE_TABLE_ADD)
    {
        ip_white_list_.insert(std::make_pair(ip, 1));
        ret = iptool.UnBanIp(ip, port);
        iAssert(ret, ("UnBanIp faild"));
    }
    else if (op == IP_WHITE_TABLE_DEL)
    {
        ip_white_list_.erase(ip);
        iptool.BanIp(ip, port);
        iAssert(ret, ("BanIp faild"));
    }

    TLOG_MSG(("OpWhiteList, op:%d, ip:%s", op, ip.c_str()));
}

bool IPWhiteList::IsIPInWhiteList(std::string ip, int port)
{
    auto iter = ip_white_list_.find(ip);
    if (iter != ip_white_list_.end())
    {
        if (iter->second == port)
        {
            return true;
        }
    }
    return false;
}

int IPWhiteList::InitWhiteList(const std::vector<std::string>& whitelist, int port)
{
    int ret = 0;

    // 同步到whitelist
    for (int i = 0; i < whitelist.size(); i++)
    {
        ret = this->OpWhiteList(IP_WHITE_TABLE_ADD, whitelist[i], port);
        iAssert(ret, ("iAssert faild"));
    }
    
    ret = IptablesTools().IptablesInit(whitelist, TCP_PORT_APPGATEWAY);
    iAssert(ret, ("IptablesInit faild"));

    return 0;
}
