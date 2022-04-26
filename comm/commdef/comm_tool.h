#pragma once

#include <string>
#include <google/protobuf/util/json_util.h>
#include "comm/commdef/comm_def.h"

#define M_TOSTR(STR) #STR

#define MSG_PROTO(Proto)                       \
    TLOG_MSG((M_TOSTR(Proto)":%s", commtool::Proto2Json(Proto).c_str()));
#define DBG_PROTO(Proto)                       \
    TLOG_DBG((M_TOSTR(Proto)":%s", commtool::Proto2Json(Proto).c_str()));

#define GET_RANDOM (rand()%1000000000+1000000000)

namespace commtool
{
    std::string GetMacAddress();
    std::string Proto2Json(const google::protobuf::Message& proto);
    time_t TimeNow_Min();
    
}

