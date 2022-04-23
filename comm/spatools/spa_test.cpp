#include "spa_tool.h"
#include "comm/tlog/tlog.h"
#include "comm/commdef/comm_def.h"

int main() {
    int ret = 0;
    /* ---------- Voucher ---------- */

    spa::SPAVoucher spaVoucher;
    spaVoucher.mutable_account()->set_acc("arashi");
    spaVoucher.mutable_account()->set_pwd("123321");
    spaVoucher.set_mac("127.0.0.1");
    spaVoucher.set_address("ChengDu");
    spaVoucher.set_is_valid(true);
    spaVoucher.set_timestamp(time(NULL));
    
    SPATools tool;
    spa::SPAVoucherPacket packet;
    ret = tool.EncryptVoucher(packet, spaVoucher, RSA_PUB_KEY_CONTROLLER);
    iAssert(ret, ("EncryptVoucher"));

    spa::SPAVoucher de_voucher;
    ret = tool.DecryptVoucher(de_voucher, packet, RSA_PRI_KEY_CONTROLLER);
    iAssert(ret, ("DecryptVoucher"));

    /* ---------- Ticket ---------- */
    spa::SPATicket spaTicket;
    spa::SPATicketPacket spaTicketPacket;
    spaTicket.set_ip("127.0.0.1");
    spaTicket.set_timestamp(time(NULL));
    ret = tool.SignTicket(spaTicketPacket, spaTicket, RSA_PRI_KEY_CONTROLLER);
    iAssert(ret, ("SignTicket"));

    ret = tool.VerifyTicket(spaTicketPacket, RSA_PUB_KEY_CONTROLLER);
    iAssert(ret, ("VerifyTicket"));

    return 0;
}
