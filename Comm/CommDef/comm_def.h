#ifndef COMM_DEF_H
#define COMM_DEF_H

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

// define

#define IP_LOCAL_ADDR "127.0.0.1"
#define IP_CONTROLLER "127.0.0.1"

#define UDP_PORT_CONTROLLER             6660
#define UDP_PORT_GATEWAY_APPLICATION    6661

#define TCP_PORT_CONTROLLER             6662
#define TCP_PORT_APPLICATION            6663
#define TCP_PORT_CLIENT                 6664




#endif
