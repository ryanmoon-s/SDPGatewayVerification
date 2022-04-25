#pragma once

#include "comm/tlog/tlog.h"
#include "comm/proto/erpc.pb.h"
#include "comm/proto/spa.pb.h"
#include "erpc_def.h"
#include "epoll_dispatcher.h"

namespace erpc {
    enum ServiceMapTable {
        // TEST
        CMD_UDP_TEST_FUNC_RECV = 1,
        CMD_RPC_TEST_FUNC_REVERSE = 2,

        // CONTROLLER
        CMD_UDP_CONTROLLER_FUNC_RECV = 101,
        CMD_RPC_CONTROLLER_FUNC_GET_ACCESS = 102,
        CMD_RPC_CONTROLLER_FUNC_REGISTER_APP = 103,

        // APPGATEWAY
        CMD_UDP_APPGATEWAY_FUNC_RECV = 201,
        CMD_RPC_APPGATEWAY_FUNC_WHITE_LIST_OP = 202,

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

    virtual int ConFuncUdpRecv(const std::string& msg, std::string from_ip, int from_port);

    virtual int ConFuncGetAccess(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp, const erpc::Extra& extra);
    
    virtual int ConFuncRegisterApp(const erpc::ConFuncRegisterAppReq& objReq, erpc::ConFuncRegisterAppRsp& objRsp, const erpc::Extra& extra);

    /***************************** APPGATEWAY *****************************/

    virtual int GateFuncUdpRecv(const std::string& msg, std::string from_ip, int from_port);

    virtual int GateFuncBlackListOp(const erpc::GateFuncBlackListOpReq& objReq, erpc::GateFuncBlackListOpRsp& objRsp, const erpc::Extra& extra);

    /***************************** APPGATEWAY *****************************/

    virtual int AppFuncHttps(const std::string& request, std::string& response);

public:
    ~ErpcService() {}
};
