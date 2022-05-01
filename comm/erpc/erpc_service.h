#pragma once

#include "comm/tlog/tlog.h"
#include "comm/proto/erpc.pb.h"
#include "comm/proto/spa.pb.h"
#include "erpc_def.h"
#include "epoll_dispatcher.h"

namespace erpc {
    enum ServiceForwardTable {
        // TEST
        CMD_UDP_TEST_FUNC_RECV = 1,
        CMD_RPC_TEST_FUNC_REVERSE = 2,

        // CONTROLLER
        CMD_UDP_CONTROLLER_FUNC_RECV = 101,
        CMD_RPC_CONTROLLER_FUNC_GET_ACCESS = 102,
        CMD_RPC_CONTROLLER_FUNC_REGISTER_APP = 103,

        // APPGATEWAY
        CMD_UDP_APPGATEWAY_FUNC_RECV = 201,
        CMD_RPC_APPGATEWAY_FUNC_BLACK_LIST_OP = 202,
        CMD_RPC_APPGATEWAY_FUNC_NOTICE = 203,

        // APPLICATION
        CMD_TCP_APPLICATION_FUNC_HTTPS = 301,
    };
    
}

class ErpcService {
public:

    /***************************** TEST FUNC *****************************/

    virtual int TestFuncUdpRecv(const std::string& msg, std::string ip, int port);
    
    virtual int TestFuncReverse(const erpc::TestFuncReverseReq& objReq, erpc::TestFuncReverseRsp& objRsp, const erpc::Extra& extra);

    /***************************** CONTROLLER *****************************/

    // UDP敲门
    virtual int ConFuncUdpRecv(const std::string& msg, std::string from_ip, int from_port);

    // 客户端获取可访问列表
    virtual int ConFuncGetAccess(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp, const erpc::Extra& extra);
    
    // 网关注册应用
    virtual int ConFuncRegisterApp(const erpc::ConFuncRegisterAppReq& objReq, erpc::ConFuncRegisterAppRsp& objRsp, const erpc::Extra& extra);

    /***************************** APPGATEWAY *****************************/

    // UDP敲门
    virtual int GateFuncUdpRecv(const std::string& msg, std::string from_ip, int from_port);

    // 黑名单操作，仅Controller调用
    virtual int GateFuncBlackListOp(const erpc::GateFuncBlackListOpReq& objReq, erpc::GateFuncBlackListOpRsp& objRsp, const erpc::Extra& extra);

    // 通知，仅APPLICATION调用
    virtual int GateFuncNotice(const erpc::GateFuncNoticeReq& objReq, erpc::GateFuncNoticeRsp& objRsp, const erpc::Extra& extra);

    /***************************** APPLICATION *****************************/

    // HTTPS 服务
    virtual int AppFuncHttps(const std::string& request, std::string& response, const erpc::Extra& extra);

public:
    ~ErpcService() {}
};
