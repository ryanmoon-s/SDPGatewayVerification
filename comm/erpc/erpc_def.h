#ifndef ERPC_DEF_H
#define ERPC_DEF_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <time.h>
#include <sys/epoll.h>

#include <memory>
#include <map>

#include "comm/commdef/comm_def.h"
#include "comm/tlog/tlog.h"
#include "comm/ssltools/ssl_tool.h"

namespace erpc_def 
{

enum SERVER_RET_ERR_DEF {
    kErrServiceNotFound = -10001,
};

enum SIMPLE_ERROR {
    kPacketLengthErr = -1001,
};


/*
组包格式 Packet
4     Header  string->protobuf
cmdid header  body
*/

struct Header {
    int ret_code;           // 返回码
    std::string ret_msg;    // 返回码的说明
};

struct Packet {
    uint32_t cmdid;     // 二进制
    Header header;      // 二进制
    std::string body;   // 原string
};

}



#endif