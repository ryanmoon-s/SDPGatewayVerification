#pragma once

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/md5.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/rand.h>
#include <string>
#include <iostream>

#include "comm/commdef//comm_def.h"
#include "comm/tlog/tlog.h"

// RSA

#define RSA_PUB_KEY_CONTROLLER "/home/shun/SDPGatewayVerification/comm/ssltools/keys/rsa/controller_pubkey.pem"
#define RSA_PRI_KEY_CONTROLLER "/home/shun/SDPGatewayVerification/comm/ssltools/keys/rsa/controller_key.pem"

#define RSA_PUB_KEY_GATEWAY "/home/shun/SDPGatewayVerification/comm/ssltools/keys/rsa/gateway_pubkey.pem"
#define RSA_PRI_KEY_GATEWAY "/home/shun/SDPGatewayVerification/comm/ssltools/keys/rsa/gateway_key.pem"

// SSL

#define SSL_CRT_CA "/home/shun/SDPGatewayVerification/comm/ssltools/keys/certs/ca.crt"

#define SSL_CRT_CONTROLLER "/home/shun/SDPGatewayVerification/comm/ssltools/keys/controller/controller.crt"
#define SSL_KEY_CONTROLLER "/home/shun/SDPGatewayVerification/comm/ssltools/keys/controller/controller.key"

#define SSL_CRT_GATEWAY "/home/shun/SDPGatewayVerification/comm/ssltools/keys/gateway/gateway.crt"
#define SSL_KEY_GATEWAY "/home/shun/SDPGatewayVerification/comm/ssltools/keys/gateway/gateway.key"

#define SSL_CRT_APPLICATION "/home/shun/SDPGatewayVerification/comm/ssltools/keys/application/application.crt"
#define SSL_KEY_APPLICATION "/home/shun/SDPGatewayVerification/comm/ssltools/keys/application/application.key"

#define SSL_CRT_CLIENT "/home/shun/SDPGatewayVerification/comm/ssltools/keys/client/client.crt"
#define SSL_KEY_CLIENT "/home/shun/SDPGatewayVerification/comm/ssltools/keys/client/client.key"


#define SSL_ERR_TLOG()                                      \
    do {                                                    \
        unsigned long err_code = ERR_get_error();           \
        char err_buf[BUFSIZ];                               \
        ERR_error_string_n(err_code, err_buf, BUFSIZ);      \
        TLOG_ERR(("SSL ERR:%s", err_buf));                  \
    }while(0)

// Less Than 0
#define SSL_iAssert_LT0(ret, Wrap)              \
    do {                                        \
        if (ret < 0) {                          \
            TLOG_ERR(Wrap);                     \
            SSL_ERR_TLOG();                     \
            return -1;                          \
        }                                       \
    } while(0)

// Not Equal 1
#define SSL_iAssert_NE1(ret, Wrap)              \
    do {                                        \
        if (ret != 1) {                         \
            TLOG_ERR(Wrap);                     \
            SSL_ERR_TLOG();                     \
            return -1;                          \
        }                                       \
    } while(0)

// NULL
#define SSL_iAssert_NULL(ret, Wrap)             \
    do {                                        \
        if (ret == NULL) {                      \
            TLOG_ERR(Wrap);                     \
            SSL_ERR_TLOG();                     \
            return -1;                          \
        }                                       \
    } while(0)
