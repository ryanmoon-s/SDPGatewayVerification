#ifndef SSL_DEF_H
#define SSL_DEF_H

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

#define PRIKEY "prikey.pem"
#define PUBKEY "pubkey.pem"
#define PUB_KEY_PATH "/home/shun/SDPGatewayVerification/Comm/SSLTools/Keys/pubkey.pem"
#define PRI_KEY_PATH "/home/shun/SDPGatewayVerification/Comm/SSLTools/Keys/key.pem"

#define SSL_ERR_TLOG()                                      \
    do {                                                    \
        unsigned long err_code = ERR_get_error();           \
        char err_buf[BUFSIZ];                               \
        ERR_error_string_n(err_code, err_buf, BUFSIZ);      \
        TLOG((ERR, "SSL ERR:%s", err_buf));                 \
    }while(0)

// Less Than 0
#define SSL_iAssert_LT0(ret, Func)              \
    do {                                        \
        if (ret < 0) {                          \
            TLOG((ERR, "Func error"));          \
            SSL_ERR_TLOG();                     \
            return -1;                          \
        }                                       \
    } while(0)

// Not Equal 1
#define SSL_iAssert_NE1(ret, Func)              \
    do {                                        \
        if (ret != 1) {                         \
            TLOG((ERR, "Func error"));          \
            SSL_ERR_TLOG();                     \
            return -1;                          \
        }                                       \
    } while(0)

// NULL
#define SSL_iAssert_NULL(ret, Func)             \
    do {                                        \
        if (ret == NULL) {                      \
            TLOG((ERR, "Func error"));          \
            SSL_ERR_TLOG();                     \
            return -1;                          \
        }                                       \
    } while(0)




#endif
