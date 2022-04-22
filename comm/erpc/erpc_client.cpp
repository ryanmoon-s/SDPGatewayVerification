#include "erpc_client.h"
#include "comm/ssltools/ssl_def.h"

int ErpcClient::UDPFuncRequest(uint32_t cmdid, const std::string& msg, const std::string& ip, int port)
{
    int ret = 0;

    ret = ErpcHandler().ClientUDPRequest(cmdid, msg, ip, port);
    iAssert(ret, ("ClientUDPRequest faild"));

    TLOG_MSG(("Client UDPFuncRequest success, ip:%s, port:%d", ip.c_str(), port));
    return 0;
}

int ErpcClient::TestFuncReverseRequest(const erpc::TestFuncReverseReq& objReq, erpc::TestFuncReverseRsp& objRsp, Header& header)
{
    Packet PacketReq;
    Packet PacketRsp;
    std::shared_ptr<SSLConnector>
        connector = std::make_shared<SSLConnector>(SSL_CRT_CONTROLLER, SSL_KEY_CONTROLLER, 0);

    // 变更 1 cmdid
    PacketReq.cmdid = erpc::CMD_RPC_TEST_FUNC_REVERSE;
    objReq.SerializeToString(&PacketReq.body);

    // 变更 2 ip port
    int ret = ErpcHandler().ClientRPCRequest(PacketReq, PacketRsp, connector, IP_CONTROLLER_PB, TCP_PORT_CONTROLLER);
    iAssert(ret, ("ClientRPCRequest"));

    objRsp.ParseFromString(PacketRsp.body);
    header = PacketRsp.header;

    // 变更 3 ip port
    TLOG_MSG(("Client TestFuncReverseRequest success, ip:%s, port:%d", IP_CONTROLLER_PB, TCP_PORT_CONTROLLER));
    return 0;
}

int ErpcClient::GateFuncWhiteListOpRequest(const erpc::GateFuncWhiteListOpReq& objReq, erpc::GateFuncWhiteListOpRsp& objRsp, Header& header)
{
    Packet PacketReq;
    Packet PacketRsp;
    std::shared_ptr<SSLConnector>
        connector = std::make_shared<SSLConnector>(SSL_CRT_CONTROLLER, SSL_KEY_CONTROLLER, 0);

    // 变更 1 cmdid
    PacketReq.cmdid = erpc::CMD_RPC_APPGATEWAY_FUNC_WHITE_LIST_OP;
    objReq.SerializeToString(&PacketReq.body);

    // 变更 2 ip port
    int ret = ErpcHandler().ClientRPCRequest(PacketReq, PacketRsp, connector, IP_APPGATEWAY_PB, TCP_PORT_APPGATEWAY);

    TLOG_DBG(("ret%d", ret));
    iAssert(ret, ("ClientRPCRequest"));
   
    objRsp.ParseFromString(PacketRsp.body);
    header = PacketRsp.header;

    // 变更 3 ip port
    TLOG_MSG(("Client TestFuncReverseRequest success, ip:%s, port:%d", IP_APPGATEWAY_PB, TCP_PORT_APPGATEWAY));
    return 0;
}

int ConFunGetAccessRequest(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp)
{
    Packet PacketReq;
    Packet PacketRsp;
    std::shared_ptr<SSLConnector>
        connector = std::make_shared<SSLConnector>(SSL_CRT_CONTROLLER, SSL_KEY_CONTROLLER, 0);
    // TODO SSL_CRT_CONTROLLER 客户端？

    // 变更 1 cmdid
    PacketReq.cmdid = erpc::CMD_RPC_CONTROLLER_FUNC_GET_ACCESS;
    objReq.SerializeToString(&PacketReq.body);

    // 变更 2 ip port
    int ret = ErpcHandler().ClientRPCRequest(PacketReq, PacketRsp, connector, IP_CONTROLLER_PB, TCP_PORT_CONTROLLER);

    TLOG_DBG(("ret%d", ret));
    iAssert(ret, ("ClientRPCRequest"));
   
    objRsp.ParseFromString(PacketRsp.body);
    header = PacketRsp.header;

    // 变更 3 ip port
    TLOG_MSG(("Client TestFuncReverseRequest success, ip:%s, port:%d", IP_CONTROLLER_PB, TCP_PORT_CONTROLLER));
    return 0;
}
