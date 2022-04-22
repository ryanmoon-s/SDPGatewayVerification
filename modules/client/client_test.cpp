#include "client.h"

int main()
{
    int ret = 0;
    spa::SPAVoucher spaVoucher;

    spaVoucher.mutable_account()->set_acc("anonymous");
    spaVoucher.mutable_account()->set_pwd("123456");
    spaVoucher.set_mac("127.0.0.1");
    spaVoucher.set_address("ChengDu");
    spaVoucher.set_is_valid(true);
    spaVoucher.set_timestamp(time(NULL));

    ret = VerifyClient().GetAccessibleAppList(spaVoucher);
    iAssert(ret, ("GetAccessibleAppList"));

    return 0;
}