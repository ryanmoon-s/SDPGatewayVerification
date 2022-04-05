#include "client.h"

int Client::GetAccessibleAppList(set<ACC_LIST_TYPE>& list, const SPAVoucher& vocher) 
{
    // SPA单包验证
    SPAPacket packet;
    int ret = SPATools::EncryptVoucher(packet, vocher);
    iAssert(ret, ("EncryptVoucher packet content:%s", packet.encrypt_spa_voucher.c_str()));

    

    return 0;
}
