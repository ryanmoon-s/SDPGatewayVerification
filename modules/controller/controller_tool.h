#pragma once

#include "comm/proto/spa.pb.h"

class SDPControllerTool {
public:
    // -1:faild  0:legal 1:illegal
    int CheckUserPermissions(const spa::SPAVoucher& spaVoucher);

};
