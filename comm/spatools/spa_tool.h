#pragma once

#include "comm/commdef/comm_def.h"
#include "comm/proto/spa.pb.h" 
#include "comm/tlog/tlog.h"
#include "comm/ssltools/ssl_tool.h"

#include <string>

class SPATools {
public:
    // SPA凭证数据包 <- SPA凭证
    int EncryptVoucher(spa::SPAVoucherPacket& spaVoucherPacket, const spa::SPAVoucher& spaVoucher, const std::string& key);

    // SPA凭证 <- SPA凭证数据包  <包含验证MD5>
    int DecryptVoucher(spa::SPAVoucher& spaVoucher, const spa::SPAVoucherPacket& spaVoucherPacket, const std::string& key);

    // SPA票据数据包 <- SPA票据
    int EncryptTicket(spa::SPATicketPacket& spaTicketPacket, const spa::SPATicket& spaTicket, const std::string& sign_pri_key, const std::string& enc_pub_key);

    // SPA票据 <- SPA票据数据包  <包含验证MD5、验证签名>
    int DecryptTicket(spa::SPATicket& spaTicket, const spa::SPATicketPacket& spaTicketPacket, const std::string& sign_pub_key, const std::string& enc_pri_key);
};
