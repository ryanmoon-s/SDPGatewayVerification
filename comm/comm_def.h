#ifndef COMM_DEF_H
#define COMM_DEF_H

#define LOCAL_IP_ADDR 127.0.0.1

#define UDP_PORT_GATEWAY_CONTROLLER  6660
#define UDP_PORT_GATEWAY_APPLICATION 6661

#define TCP_PORT_APPLICATION         6662
#define TCP_PORT_CLIENT              6663

// linux sys includes
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


#endif
