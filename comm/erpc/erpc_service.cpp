#include "erpc_service.h"

#include <algorithm>

int erpc_service::FuncReverse(const erpc::FuncReverseReq& objReq, erpc::FuncReverseRsp& objRsp) {
    std::string str = objReq.str();
    std::reverse(str.begin(), str.end());
    objRsp.set_str(str);

    TLOG_MSG(("objReq:%s, objRsp:%s", objReq.str().c_str(), objRsp.str().c_str()));
    return 0;
}
