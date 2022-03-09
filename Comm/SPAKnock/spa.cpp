#include "spa.h"

/*
 * account,password,ip,address,timestamp,is_valid(0,1)
 */
SPAPacket EncryptVoucher(SPAVoucher voucher) {
    std::string str;
    str.append(voucher.u_acc.user_account + ",");
    str.append(voucher.u_acc.user_password + ",");
    str.append(voucher.u_ip + ",");
    str.append(voucher.u_address + ",");
    str.append(std::to_string(voucher.u_timestamp) + ",");
    str.append(voucher.is_valid ? "1" : "0");

    SPAPacket packet;
    packet.encrypt_spa_voucher = str;
    return packet;
}

SPAVoucher DecryptVoucher(SPAPacket packet) {
    SPAVoucher voucher;



    return voucher;
}
