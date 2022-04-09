#ifndef SPA_TOOL_H
#define SPA_TOOL_H

#include "comm/commdef/comm_def.h"
#include "comm/proto/spa.pb.h" 

#include <string>

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#define SeparatorForStr "///@XiHuaUniversity///"

class SPATools {
public:
    // SPA凭证 -> SPA数据包
    int EncryptVoucher(spa::SPAPacket& packet, const spa::SPAVoucher& voucher);
    // SPA数据包 -> SPA凭证
    int DecryptVoucher(spa::SPAVoucher& voucher, const spa::SPAPacket& packet);
};





#endif

