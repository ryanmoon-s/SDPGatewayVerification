#pragma once

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

namespace erpc
{

// 错误码
enum SERVER_RET_ERR_DEF {
    kErrServiceNotFound = -501,
};
enum SIMPLE_ERROR_DEF {
    kPacketLengthErr = -1001,
    kIpNotInWhiteTable = -1002,
};

// 服务端身份识别
enum SERVER_Identification {
    ID_CONTROLLER = 1,
    ID_GATEWAY = 2,
    ID_APPLICATION = 3,
};

/*
    组包格式
    cmdid header body
    4     4+50   left
*/

struct Header {
    int ret_code;           // 返回码       4字节
    std::string ret_msg;    // 返回码的说明 50字节
};
#define HEADER_RET_MSG_LIMIT_SIZE 50

struct Packet {
    uint32_t cmdid;     // 二进制
    Header header;      // 二进制
    std::string body;   // 原string
};

struct SocketInfo {
    std::string ip;
    int port;
};

// 附加给service
struct Extra {
    SocketInfo socket_info;
};





}
