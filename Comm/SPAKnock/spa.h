#ifndef SPA_H
#define SPA_H

#include "comm/comm_def.h"
#include <string>

#include <stdio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

// SPA 凭证
struct SPAVoucher {
    struct account {
        std::string user_account;
        std::string user_password;
    };
    account     u_acc;
    std::string u_ip;
    std::string u_address;
    int64_t     u_timestamp;
    bool        is_valid = false;
};

// SPA数据包
// = 加密的SPA凭证 + SPA凭证摘要
struct SPAPacket {
    std::string encrypt_spa_voucher;
};

// SPA凭证 -> SPA数据包
SPAPacket EncryptVoucher(SPAVoucher voucher);
// SPA数据包 -> SPA凭证
SPAVoucher DecryptVoucher(SPAPacket packet);
// SPA敲门
int SPAKnocking(SPAPacket packet);


#endif

