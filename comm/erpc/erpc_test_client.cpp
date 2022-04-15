#include "erpc_client.h"

int main() {
    int ret = 0;
    erpc::TestFuncReq req;
    erpc::TestFuncRsp rsp;
    Header header;
    ret = Client().TestFuncClient(req, rsp, header);
    iAssert(ret, ("TestFuncClient"));
    TLOG_DBG(("TestFunc Sucess: req.str:%s, rsp.str:%s", req.str().c_str(), rsp.str().c_str()));

    return 0;
}