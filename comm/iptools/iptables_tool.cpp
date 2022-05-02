#include "iptables_tool.h"
#include "comm/tlog/tlog.h"

#ifdef IP_TABLES_EXECUTE
    #define SYSTEM_EXEC_CMD ret=system(cmd.c_str())
#else
    #define SYSTEM_EXEC_CMD TLOG_MSG((" == == Dont exec iptables cmd == =="));
#endif

IptablesTools::IptablesTools()
{
    int ret = 0;
    std::string cmd;

    // 清除所有规则
    cmd = "iptables -Z";
    _ExecuteCmd(cmd);
    iAssertNoReturn(ret, ("exec: clear all rules -Z"));

    cmd = "iptables -X";
    _ExecuteCmd(cmd);
    iAssertNoReturn(ret, ("exec: clear all rules -X"));

    cmd = "iptables -F";
    _ExecuteCmd(cmd);
    iAssertNoReturn(ret, ("exec: clear all rules -F"));

    TLOG_MSG((" == iptables init, clear all rules == "));
}

int IptablesTools::IptablesInit(const std::vector<std::string>& whitelist, int port)
{
    int ret = 0;
    std::string cmd;

    // 封禁此端口所有IP
    cmd = "iptables -I INPUT -p tcp --dport " + std::to_string(port) + " -j DROP";
    _ExecuteCmd(cmd);
    iAssert(ret, ("exec: forbid all ip, port:%d", port));

    // 打开此端口白名单IP
    for (int i = 0; i < whitelist.size(); i++)
    {
        ret = UnbanUser(whitelist[i], port);
        iAssert(ret, ("UnbanUser"));
    }

    return 0;
}

int IptablesTools::BanUser(const std::string& ip, int port)
{
    int ret = 0;
     
    std::string cmd = "iptables -D INPUT -s " + ip + " -p tcp --dport " + std::to_string(port) + " -j ACCEPT";
    _ExecuteCmd(cmd);
    TLOG_DBG(("BanUser exec, ip:%s, port:%d", ip.c_str(), port));

    return ret;
}

int IptablesTools::UnbanUser(const std::string& ip, int port)
{
    int ret = 0;

    std::string cmd = "iptables -I INPUT -s " + ip + " -p tcp --dport " + std::to_string(port) + " -j ACCEPT";
    _ExecuteCmd(cmd);
    TLOG_DBG(("UnbanUser exec, ip:%s, port:%d", ip.c_str(), port));

    return ret;
}

int IptablesTools::_ExecuteCmd(std::string cmd)
{
    int ret = 0;
    SYSTEM_EXEC_CMD;
    TLOG_DBG(("ret:%d, cmd:%s", ret, cmd.c_str()));
    return (ret == 0 ? 0 : -1);
}


/***************************** White List *****************************/


// op: IP_WHITE_LIST_OP
int IPWhiteList::OpWhiteList(int op, std::string ip, int port)
{
    int ret = 0;
    bool has_rule = IsIPInWhiteList(ip, port);

    if (op == IP_WHITE_LIST_ADD)
    {
        // 避免重复操作
        if (has_rule)
        {
            TLOG_ERR(("not add, rule exists"));
            return -1;
        }
        ip_white_list_.insert(std::make_pair(ip, port));
        ret = iptool_.UnbanUser(ip, port);
        iAssert(ret, ("UnbanUser faild"));
    }

    if (op == IP_WHITE_LIST_DEL)
    {
        if (!has_rule)
        {
            TLOG_ERR(("not del, rule not exists"));
            return -1;
        }
        ip_white_list_.erase(ip);
        ret = iptool_.BanUser(ip, port);
        iAssert(ret, ("BanUser faild"));
    }

    TLOG_MSG(("success, op:%d, ip:%s", op, ip.c_str()));
    return 0;
}

bool IPWhiteList::IsIPInWhiteList(std::string ip, int port)
{
    auto iter = ip_white_list_.find(ip);
    if (iter != ip_white_list_.end() && iter->second == port)
    {
        return true;
    }
    return false;
}

int IPWhiteList::InitWhiteList(const std::vector<std::string>& whitelist, int port)
{
    int ret = 0;

    // 同步到whitelist
    for (int i = 0; i < whitelist.size(); i++)
    {
        ip_white_list_.insert(std::make_pair(whitelist[i], 1));
    }
    
    ret = iptool_.IptablesInit(whitelist, port);
    iAssert(ret, ("IptablesInit faild"));

    TLOG_DBG(("InitWhiteList success, size:%d, port:%d", whitelist.size(), port));
    return 0;
}
