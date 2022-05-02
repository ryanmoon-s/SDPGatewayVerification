#pragma once

#include <string>

// linux sys includes
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <time.h>


// 线上环境
// #define ONLINE_ENVIRONMENT


// 服务端监听 用内网IP
// 客户端访问 用公网IP、访问过来是公网IP

#define IP_LOCAL_ADDR "127.0.0.1"

#define IP_ALIYUN_IN "172.17.55.219"    // 内网IP
#define IP_ALIYUN_PB "47.111.5.82"      // 公网IP

#define IP_TENCLOUD_IN "10.0.20.9"
#define IP_TENCLOUD_PB "43.138.134.77"

#define IP_BAIDUYUN_IN "192.168.0.4"
#define IP_BAIDUYUN_PB "106.13.211.246"

// IP部署
#ifdef ONLINE_ENVIRONMENT   // 线上环境

// Client
#define IP_CLIENT_IN IP_ALIYUN_IN
#define IP_CLIENT_PB IP_ALIYUN_PB

// Controller
#define IP_CONTROLLER_IN IP_TENCLOUD_IN
#define IP_CONTROLLER_PB IP_TENCLOUD_PB

// Gateway Application
#define IP_APPGATEWAY_IN IP_BAIDUYUN_IN
#define IP_APPGATEWAY_PB IP_BAIDUYUN_PB
#define IP_APPLICATION_IN IP_BAIDUYUN_IN
#define IP_APPLICATION_PB IP_BAIDUYUN_PB

// 定义则执行iptables封禁
#define IP_TABLES_EXECUTE

#else   // 测试环境

#define IP_CLIENT_IN IP_BAIDUYUN_IN
#define IP_CLIENT_PB IP_BAIDUYUN_PB

#define IP_CONTROLLER_IN IP_BAIDUYUN_IN
#define IP_CONTROLLER_PB IP_BAIDUYUN_PB

#define IP_APPGATEWAY_IN IP_BAIDUYUN_IN
#define IP_APPGATEWAY_PB IP_BAIDUYUN_PB
#define IP_APPLICATION_IN IP_BAIDUYUN_IN
#define IP_APPLICATION_PB IP_BAIDUYUN_PB

#endif

// 端口部署
// 服务器端口开放 6000-6020

#define UDP_PORT_CONTROLLER             6000
#define UDP_PORT_APPGATEWAY             6001

#define TCP_PORT_CONTROLLER             6002
#define TCP_PORT_APPGATEWAY             6003

#define TCP_PORT_APPLICATION            6004
