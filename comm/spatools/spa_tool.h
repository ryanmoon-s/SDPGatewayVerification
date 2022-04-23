#pragma once

#include "comm/commdef/comm_def.h"
#include "comm/proto/spa.pb.h" 
#include "comm/tlog/tlog.h"
#include "comm/ssltools/ssl_tool.h"

#include <string>

class SPATools {
public:
    // SPA凭证 -> SPA数据包
    int EncryptVoucher(spa::SPAVoucherPacket& spaVoucherPacket, const spa::SPAVoucher& spaVoucher, const std::string& key);
    
    // SPA凭证 <- SPA数据包
    int DecryptVoucher(spa::SPAVoucher& spaVoucher, const spa::SPAVoucherPacket& spaVoucherPacket, const std::string& key);

    // SPA票据 -> 签名 -> SPA数据包
    int SignTicket(spa::SPATicketPacket& spaTicketPacket, const spa::SPATicket& spaTicket, const std::string& key);

    // SPA数据包: 验证Ticket签名是否属实
    int VerifyTicket(spa::SPATicketPacket& spaTicketPacket, const std::string& key);
};
