#include "erpc_client.h"
#include "comm/ssltools/ssl_def.h"

int ErpcClient::UDPFuncRequest(uint32_t cmdid, const std::string& msg, const std::string& ip, int port)
{
    int ret = 0;

    ret = ErpcHandler().ClientUDPRequest(cmdid, msg, ip, port);
    iAssert(ret, ("ClientUDPRequest faild"));

    return 0;
}

int ErpcClient::TestFuncReverseRequest(const erpc::TestFuncReverseReq& objReq, erpc::TestFuncReverseRsp& objRsp, Header& header)
{
    Packet PacketReq;
    Packet PacketRsp;
    std::shared_ptr<SSLConnector>
        connector = std::make_shared<SSLConnector>(SSL_CRT_CLIENT, SSL_KEY_CLIENT, 0);

    // 变更 1
    PacketReq.cmdid = erpc::CMD_RPC_TEST_FUNC_REVERSE;
    objReq.SerializeToString(&PacketReq.body);

    // 变更 2
    int ret = ErpcHandler().ClientRPCRequest(PacketReq, PacketRsp, connector, IP_CONTROLLER, TCP_PORT_CONTROLLER);
    iAssert(ret, ("ClientRPCRequest"));

    objRsp.ParseFromString(PacketRsp.body);
    header = PacketRsp.header;

    return 0;
}

int ErpcClient::GateFuncWhiteListOpRequest(const erpc::GateFuncWhiteListOpReq& objReq, erpc::GateFuncWhiteListOpRsp& objRsp, Header& header)
{
    Packet PacketReq;
    Packet PacketRsp;
    std::shared_ptr<SSLConnector>
        connector = std::make_shared<SSLConnector>(SSL_CRT_CLIENT, SSL_KEY_CLIENT, 0);

    // 变更 1
    PacketReq.cmdid = erpc::CMD_RPC_APPGATEWAY_FUNC_WHITE_LIST_OP;
    objReq.SerializeToString(&PacketReq.body);

    // 变更 2
    int ret = ErpcHandler().ClientRPCRequest(PacketReq, PacketRsp, connector, IP_CONTROLLER, TCP_PORT_APPGATEWAY);
    iAssert(ret, ("ClientRPCRequest"));

    objRsp.ParseFromString(PacketRsp.body);
    header = PacketRsp.header;
 
    return 0;
}
