#pragma once

#include "comm/erpc/erpc_config.h"

enum IP_WHITE_TABLE_OP {
    IP_WHITE_TABLE_ADD = 1,
    IP_WHITE_TABLE_DEL = 2,
};

class SDPControllerErpcIpWhiteListImpl: public ErpcIpWhiteList {
public:
    void OpIpWhiteList(int op, std::string ip);

public:
    virtual bool IsIpInWhiteList(std::string ip) override;

private:
    std::map<std::string, int> ip_white_list_;
};
