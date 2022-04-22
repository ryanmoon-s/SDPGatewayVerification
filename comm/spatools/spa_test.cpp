#include "spa_tool.h"
#include "comm/tlog/tlog.h"
#include "comm/commdef/comm_def.h"

int main() {
    spa::SPAVoucher voucher;
    voucher.mutable_account()->set_acc("arashi");
    voucher.mutable_account()->set_pwd("123321");
    voucher.set_mac("127.0.0.1");
    voucher.set_address("ChengDu");
    voucher.set_is_valid(true);
    voucher.set_timestamp(time(NULL));
    
    SPATools tool;
    spa::SPAVoucherPacket packet;
    tool.EncryptVoucher(packet, voucher);

    spa::SPAVoucher de_voucher;
    tool.DecryptVoucher(de_voucher, packet);
    // 再次加密，可以看到中途解析成str的voucher数据
    tool.EncryptVoucher(packet, de_voucher);

    return 0;
}
