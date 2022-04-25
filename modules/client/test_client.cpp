#include "sdp_verify_client.h"
#include "sdp_access_client.h"

int Access_Test(std::string acc, std::string passwd);

int main()
{
    int ret = 0;

    ret = Access_Test("xiaoming", "123456");
    iAssert(ret, ("Access_Test faild"));
    TLOG_DBG(("======================== xiaoming ========================"));
    
    ret = Access_Test("xiaohong", "123456");
    iAssert(ret, ("Access_Test faild"));
    TLOG_DBG(("======================== xiaohong ========================"));

    ret = Access_Test("xiaobai", "123456789");
    iAssert(ret, ("Access_Test faild"));
    TLOG_DBG(("======================== xiaobai ========================"));


    return 0;
}

int Access_Test(std::string acc, std::string passwd)
{
    int ret = 0;
    spa::SPAVoucher spaVoucher;

    spaVoucher.mutable_account()->set_acc(acc);
    spaVoucher.mutable_account()->set_pwd(passwd);
    spaVoucher.set_mac(commtool::GetMacAddress());
    spaVoucher.set_address("ChengDu");
    spaVoucher.set_timestamp(time(NULL));
    spaVoucher.set_random(GET_RANDOM);

    std::vector<erpc::AccessItem> list;
    ret = VerifyClient().GetAccessibleAppList(list, spaVoucher);
    iAssert(ret, ("GetAccessibleAppList faild"));

    ret = AccessClient().AccessApplication_HTTPS(list);
    iAssert(ret, ("AccessApplication_HTTPS faild"));

    return 0;
}