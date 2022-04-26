#pragma once

#include <string>
#include <vector>
#include "comm/commdef/comm_def.h"

class IptablesTools {
public:
    // 1、屏蔽此端口所有IP
    // 2、加入指定白名单允许策略
    int IptablesInit(const std::vector<std::string>& whitelist, int port);
    // 禁止访问 DROP IP
    int BanUser(const std::string& ip, int port);
    // 允许访问 ACCEPT IP
    int UnbanUser(const std::string& ip, int port);

public:
    // 构造时清除所有规则
    IptablesTools();

private:
    int _ExecuteCmd(std::string cmd);
};

#include <map>

enum IP_TABLE_LIST_OP {
    IP_WHITE_LIST_ADD = 1,
    IP_WHITE_LIST_DEL = 2,
    IP_BLACK_LIST_ADD = 3,
    IP_BLACK_LIST_DEL = 4,
};

// 与iptables同步
class IPWhiteList {
public:
    
    int InitWhiteList(const std::vector<std::string>& whitelist, int port);

    int OpWhiteList(int op, std::string ip, int port);
    bool IsIPInWhiteList(std::string ip, int port);

private:
    std::map<std::string, int> ip_white_list_;
    IptablesTools iptool_;
};
