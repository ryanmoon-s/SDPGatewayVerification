#ifndef SPA_TOOL_H
#define SPA_TOOL_H

#include "Comm/comm_def.h"
#include <string>

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
    uint32_t    u_timestamp;
    bool        is_valid = false;
};

// SPA数据包
// = 加密的SPA凭证 + SPA凭证摘要
struct SPAPacket {
    std::string encrypt_spa_voucher;
};

class SPATools {
public:
    // SPA凭证 -> SPA数据包
    int EncryptVoucher(SPAPacket& packet, const SPAVoucher& voucher);
    // SPA数据包 -> SPA凭证
    int DecryptVoucher(SPAVoucher& voucher, const SPAPacket& packet);
    // SPA敲门
    int SPAKnocking(SPAPacket packet);

private:
    int _ParseVoucherFromStr(SPAVoucher& voucher, const std::string& str); 
    int _GetValueFromStr(std::string& value, const std::string& str, const std::string& key);

};

#define SeparatorForStr "///@XiHuaUniversity///"


#endif

