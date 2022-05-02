#include "spa_tool.h"
#include "comm/tlog/tlog.h"
#include "comm/commdef/comm_def.h"
#include "comm/commdef/comm_tool.h"

using namespace commtool;

int main() {
    int ret = 0;
    /* ---------- Voucher ---------- */

    spa::SPAVoucher spaVoucher;
    spaVoucher.mutable_account()->set_acc("arashi");
    spaVoucher.mutable_account()->set_pwd("123321");
    spaVoucher.set_mac("127.0.0.1");
    spaVoucher.set_address("ChengDu");
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
    spa::SPATicket spaTicket_de;
    spa::SPATicketPacket spaTicketPacket;
    spaTicket.set_ip("127.0.0.1");
    spaTicket.set_timestamp(time(NULL));
    ret = tool.EncryptTicket(spaTicketPacket, spaTicket, RSA_PRI_KEY_CONTROLLER, RSA_PUB_KEY_GATEWAY);
    iAssert(ret, ("EncryptTicket"));
    DBG_PROTO(spaTicketPacket);

    ret = tool.DecryptTicket(spaTicket_de, spaTicketPacket, RSA_PUB_KEY_CONTROLLER, RSA_PRI_KEY_GATEWAY);
    iAssert(ret, ("DecryptTicket"));
    DBG_PROTO(spaTicket_de);

    return 0;
}
