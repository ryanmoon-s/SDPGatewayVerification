#include "client.h"

int main()
{
    int ret = 0;
    spa::SPAVoucher voucher;

    voucher.mutable_account()->set_acc("anonymous");
    voucher.mutable_account()->set_pwd("123456");
    voucher.set_ip("127.0.0.1");
    voucher.set_address("ChengDu");
    voucher.set_is_valid(true);
    voucher.set_timestamp(time(NULL));

    ret = VerifyClient().GetAccessibleAppList(voucher);
    iAssert(ret, ("GetAccessibleAppList"));

    return 0;
}