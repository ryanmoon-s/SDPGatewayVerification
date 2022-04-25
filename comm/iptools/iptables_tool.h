#pragma once

#include <string>
#include <vector>
#include "comm/commdef/comm_def.h"

class IptablesTools {
public:
    // DROP 所有 port
    int IptablesInit(const std::vector<std::string>& whitelist, int port);
    // DROP IP
    int BanIp(const std::string& ip, int port);
    // ACCEPT IP
    int UnBanIp(const std::string& ip, int port);

private:
    int _BanAll(int port);
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
};
