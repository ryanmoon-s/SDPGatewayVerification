#include "spa_tool.h"
#include "Comm/TLog/tlog.h"
#include "Comm/comm_def.h"

int main() {
    SPAVoucher voucher;
    voucher.u_acc.user_account = "arashi";
    voucher.u_acc.user_password = "pass";
    voucher.u_ip = "127.0.0.1";
    voucher.u_address = "ChengDu";
    voucher.is_valid = true;
    time_t time_now;
    time(&time_now);
    voucher.u_timestamp = time_now;

    SPATools tool;
    SPAPacket packet;
    tool.EncryptVoucher(packet, voucher);

    SPAVoucher de_voucher;
    tool.DecryptVoucher(de_voucher, packet);
    // 再次加密，可以看到中途解析成str的voucher数据
    tool.EncryptVoucher(packet, de_voucher);

    
    return 0;
}
