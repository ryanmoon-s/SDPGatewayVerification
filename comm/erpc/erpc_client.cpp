#include "erpc_client.h"
#include "comm/ssltools/ssl_def.h"

// 一个服务一个Client
int Client::TestFuncClient(const erpc::TestFuncReq& req, erpc::TestFuncRsp& rsp, Header& header)
{
    Packet PacketReq;
    Packet PacketRsp;
    std::shared_ptr<SSLConnector> 
        connector = std::make_shared<SSLConnector>(SSL_CRT_CLIENT, SSL_KEY_CLIENT, SSL_SELECT_CLIENT);

    PacketReq.cmdid = CMD_TEST_FUNC;
    req.SerializeToString(&PacketReq.body);

    TLOG_DBG(("size:%d, str:%s", PacketReq.body.size(), PacketReq.body.c_str()));
    int ret = ErpcHandler().ClientRequest(PacketReq, PacketRsp, connector, IP_CONTROLLER, TCP_PORT_CONTROLLER);
    iAssert(ret, ("ClientRequest"));

    rsp.ParseFromString(PacketRsp.body);
    header = PacketRsp.header;

    return 0;
}