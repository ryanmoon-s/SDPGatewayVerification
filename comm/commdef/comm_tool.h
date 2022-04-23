#pragma once

#include <string>
#include <google/protobuf/util/json_util.h>

#include "comm/commdef/comm_def.h"

namespace commtool
{
    int GetMacAddress(std::string& mac_address);
    std::string Proto2Json(const google::protobuf::Message& proto);

    
}
