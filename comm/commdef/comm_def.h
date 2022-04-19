#pragma once

// linux sys includes
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

#define IP_LOCAL_ADDR "127.0.0.1"
#define IP_CONTROLLER "127.0.0.1"

// aliyun 服务器 开放端口
#define UDP_PORT_CONTROLLER             6000
#define UDP_PORT_GATEWAY_APPLICATION    6001

#define TCP_PORT_CONTROLLER             6002
#define TCP_PORT_APPLICATION            6003
#define TCP_PORT_CLIENT                 6004
