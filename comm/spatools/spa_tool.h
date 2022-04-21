#pragma once

#include "comm/commdef/comm_def.h"
#include "comm/proto/spa.pb.h" 

#include <string>

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

class SPATools {
public:
    // SPA凭证 -> SPA数据包
    int EncryptVoucher(spa::SPAVoucherPacket& packet, const spa::SPAVoucher& voucher);
    // SPA数据包 -> SPA凭证
    int DecryptVoucher(spa::SPAVoucher& voucher, const spa::SPAVoucherPacket& packet);
};

