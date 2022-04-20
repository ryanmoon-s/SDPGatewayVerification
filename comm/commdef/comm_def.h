#pragma once

// linux sys includes
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <time.h>

// 服务端监听 用内网IP
// 客户端访问 用公网IP、访问过来是公网IP

#define IP_LOCAL_ADDR "127.0.0.1"

#define IP_ALIYUN_IN "172.17.55.219"    // 内网IP
#define IP_ALIYUN_PB "47.111.5.82"      // 公网IP

#define IP_TENCLOUD_IN "10.0.20.9"
#define IP_TENCLOUD_PB "43.138.134.77"

#define IP_CONTROLLER_IN IP_ALIYUN_IN
#define IP_CONTROLLER_PB IP_ALIYUN_PB

#define IP_APPGATEWAY_IN IP_ALIYUN_IN
#define IP_APPGATEWAY_PB IP_ALIYUN_PB


// 服务器端口开放 6000-6020

#define UDP_PORT_CONTROLLER             6000
#define UDP_PORT_APPGATEWAY             6001

#define TCP_PORT_CONTROLLER             6002
#define TCP_PORT_APPGATEWAY             6003
