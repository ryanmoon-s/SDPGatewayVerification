#pragma once

#include "comm/commdef/comm_def.h"
#include "comm/proto/spa.pb.h" 
#include "comm/tlog/tlog.h"
#include "comm/ssltools/ssl_tool.h"

#include <string>

class SPATools {
public:
    // SPA数据包 <- SPA凭证
    int EncryptVoucher(spa::SPAVoucherPacket& spaVoucherPacket, const spa::SPAVoucher& spaVoucher, const std::string& key);
    
    // SPA凭证 <- SPA数据包  <包含验证>
    int DecryptVoucher(spa::SPAVoucher& spaVoucher, const spa::SPAVoucherPacket& spaVoucherPacket, const std::string& key);

    // SPA数据包 <- SPA票据+签名
    int SignTicket(spa::SPATicketPacket& spaTicketPacket, const spa::SPATicket& spaTicket, const std::string& key);

    // SPA数据包: 验证Ticket签名是否属于某人
    int VerifyTicket(spa::SPATicketPacket& spaTicketPacket, const std::string& key);
};
