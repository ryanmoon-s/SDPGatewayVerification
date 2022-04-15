#include "erpc_service.h"

#include <algorithm>

int erpc_service::TestFunc(const erpc::TestFuncReq& objReq, erpc::TestFuncRsp& objRsp, Header& header) {
    std::string str = objReq.str();
    std::reverse(str.begin(), str.end());
    objRsp.set_str(str);

    TLOG_DBG(("TestFunc objReq:%s, objRsp:%s", objReq.str().c_str(), objRsp.str().c_str()));
    return 0;
}

