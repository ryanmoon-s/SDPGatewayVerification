#include "erpc_client.h"
#include "comm/ssltools/ssl_def.h"

int Client::FuncReverseClient(const erpc::FuncReverseReq& req, erpc::FuncReverseRsp& rsp, Header& header)
{
    Packet PacketReq;
    Packet PacketRsp;
    std::shared_ptr<SSLConnector> 
        connector = std::make_shared<SSLConnector>(SSL_CRT_CLIENT, SSL_KEY_CLIENT, 0);

    // 变更 1
    PacketReq.cmdid = CMD_FUNC_REVERSE;
    req.SerializeToString(&PacketReq.body);

    // 变更 2
    int ret = ErpcHandler().ClientRequest(PacketReq, PacketRsp, connector, IP_CONTROLLER, TCP_PORT_CONTROLLER);
    iAssert(ret, ("ClientRequest"));

    rsp.ParseFromString(PacketRsp.body);
    header = PacketRsp.header;

    return 0;
}