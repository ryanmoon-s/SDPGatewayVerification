#pragma once

#include "comm/proto/spa.pb.h"

class SDPControllerTool {
public:
    // -1:faild  0:legal 1:illegal
    int AddUserBlackList(int op, const std::string& ip, int port);

};
