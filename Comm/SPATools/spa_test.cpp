#include "spa_tool.h"
#include "Comm/TLog/tlog.h"
#include "Comm/CommDef/comm_def.h"

int main() {
    spa::SPAVoucher voucher;
    voucher.mutable_acc()->set_user_account("arashi");
    voucher.mutable_acc()->set_user_password("123321");
    voucher.set_ip("127.0.0.1");
    voucher.set_address("ChengDu");
    voucher.set_is_valid(true);
    time_t time_now = time(NULL);
    voucher.set_timestamp(time_now);

    SPATools tool;
    spa::SPAPacket packet;
    tool.EncryptVoucher(packet, voucher);

    spa::SPAVoucher de_voucher;
    tool.DecryptVoucher(de_voucher, packet);
    // // 再次加密，可以看到中途解析成str的voucher数据
    tool.EncryptVoucher(packet, de_voucher);

    return 0;
}
