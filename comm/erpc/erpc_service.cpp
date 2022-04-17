#include "erpc_service.h"

#include <algorithm>

int erpc_service::FuncReverse(const erpc::FuncReverseReq& objReq, erpc::FuncReverseRsp& objRsp, Header& header) {
    std::string str = objReq.str();
    std::reverse(str.begin(), str.end());
    objRsp.set_str(str);

    header.ret_code = 20;
    header.ret_msg = "no problem";
    TLOG_DBG(("FuncReverse objReq:%s, objRsp:%s", objReq.str().c_str(), objRsp.str().c_str()));
    return 0;
}

