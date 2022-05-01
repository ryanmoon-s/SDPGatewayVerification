#include "erpc_client.h"
#include "comm/ssltools/ssl_def.h"

int ErpcClient::UDPFuncRequest(uint32_t cmdid, const std::string& msg, const std::string& from_ip, int from_port)
{
    int ret = 0;

    ret = ErpcHandler().ClientUDPRequest(cmdid, msg, from_ip, from_port);
    iAssert(ret, ("ClientUDPRequest faild"));
    
    return 0;
}

int ErpcClient::TestFuncReverseRequest(const erpc::TestFuncReverseReq& objReq, erpc::TestFuncReverseRsp& objRsp, Header& header)
{
    Packet PacketReq;
    Packet PacketRsp;
    std::shared_ptr<SSLConnector>
        connector = std::make_shared<SSLConnector>(cert_, key_, 0);

    // 变更 1 cmdid
    PacketReq.cmdid = erpc::CMD_RPC_TEST_FUNC_REVERSE;
    objReq.SerializeToString(&PacketReq.body);

    // 变更 2 ip port
    int ret = ErpcHandler().ClientRPCRequest(PacketReq, PacketRsp, connector, IP_CONTROLLER_PB, TCP_PORT_CONTROLLER);
    iAssert(ret, ("ClientRPCRequest"));

    objRsp.ParseFromString(PacketRsp.body);
    header = PacketRsp.header;

    // 变更 3 name ip port
    TLOG_MSG(("Client TestFuncReverseRequest success, ip:%s, port:%d", IP_CONTROLLER_PB, TCP_PORT_CONTROLLER));
    return 0;
}

int ErpcClient::GateFuncBlackListOpRequest(const erpc::GateFuncBlackListOpReq& objReq, erpc::GateFuncBlackListOpRsp& objRsp, Header& header)
{
    Packet PacketReq;
    Packet PacketRsp;
    std::shared_ptr<SSLConnector>
        connector = std::make_shared<SSLConnector>(cert_, key_, 0);

    // 变更 1 cmdid
    PacketReq.cmdid = erpc::CMD_RPC_APPGATEWAY_FUNC_BLACK_LIST_OP;
    objReq.SerializeToString(&PacketReq.body);

    // 变更 2 ip port
    int ret = ErpcHandler().ClientRPCRequest(PacketReq, PacketRsp, connector, IP_APPGATEWAY_PB, TCP_PORT_APPGATEWAY);
    iAssert(ret, ("ClientRPCRequest"));
   
    objRsp.ParseFromString(PacketRsp.body);
    header = PacketRsp.header;

    // 变更 3 name ip port
    TLOG_MSG(("Client GateFuncBlackListOpRequest success, ip:%s, port:%d", IP_APPGATEWAY_PB, TCP_PORT_APPGATEWAY));
    return 0;
}

int ErpcClient::GateFuncNoticeRequest(const erpc::GateFuncNoticeReq& objReq, erpc::GateFuncNoticeRsp& objRsp, Header& header)
{
    Packet PacketReq;
    Packet PacketRsp;
    std::shared_ptr<SSLConnector>
        connector = std::make_shared<SSLConnector>(cert_, key_, 0);

    // 变更 1 cmdid
    PacketReq.cmdid = erpc::CMD_RPC_APPGATEWAY_FUNC_NOTICE;
    objReq.SerializeToString(&PacketReq.body);

    // 变更 2 ip port
    int ret = ErpcHandler().ClientRPCRequest(PacketReq, PacketRsp, connector, IP_APPGATEWAY_PB, TCP_PORT_APPGATEWAY);
    iAssert(ret, ("ClientRPCRequest"));
   
    objRsp.ParseFromString(PacketRsp.body);
    header = PacketRsp.header;

    // 变更 3 name ip port
    TLOG_MSG(("Client GateFuncNoticeRequest success, ip:%s, port:%d", IP_APPGATEWAY_PB, TCP_PORT_APPGATEWAY));
    return 0;
}


int ErpcClient::ConFuncGetAccessRequest(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp, Header& header)
{
    Packet PacketReq;
    Packet PacketRsp;
    std::shared_ptr<SSLConnector>
        connector = std::make_shared<SSLConnector>(cert_, key_, 0);

    // 变更 1 cmdid
    PacketReq.cmdid = erpc::CMD_RPC_CONTROLLER_FUNC_GET_ACCESS;
    objReq.SerializeToString(&PacketReq.body);

    // 变更 2 ip port
    int ret = ErpcHandler().ClientRPCRequest(PacketReq, PacketRsp, connector, IP_CONTROLLER_PB, TCP_PORT_CONTROLLER);
    iAssert(ret, ("ClientRPCRequest"));
   
    objRsp.ParseFromString(PacketRsp.body);
    header = PacketRsp.header;

    // 变更 3 name ip port
    TLOG_MSG(("Client ConFuncGetAccessRequest success, ip:%s, port:%d", IP_CONTROLLER_PB, TCP_PORT_CONTROLLER));
    return 0;
}

int ErpcClient::ConFuncRegisterAppRequest(const erpc::ConFuncRegisterAppReq& objReq, erpc::ConFuncRegisterAppRsp& objRsp, Header& header)
{
    Packet PacketReq;
    Packet PacketRsp;
    std::shared_ptr<SSLConnector>
        connector = std::make_shared<SSLConnector>(cert_, key_, 0);

    // 变更 1 cmdid
    PacketReq.cmdid = erpc::CMD_RPC_CONTROLLER_FUNC_REGISTER_APP;
    objReq.SerializeToString(&PacketReq.body);

    // 变更 2 ip port
    int ret = ErpcHandler().ClientRPCRequest(PacketReq, PacketRsp, connector, IP_CONTROLLER_PB, TCP_PORT_CONTROLLER);
    iAssert(ret, ("ClientRPCRequest"));

    objRsp.ParseFromString(PacketRsp.body);
    header = PacketRsp.header;

    // 变更 3 name ip port
    TLOG_MSG(("Client ConFuncRegisterAppRequest success, ip:%s, port:%d", IP_CONTROLLER_PB, TCP_PORT_CONTROLLER));
    return 0;
}

int ErpcClient::AppFuncHttpsRequest(const std::string& request, std::string& response, const std::string& ip, int port)
{
    std::shared_ptr<SSLConnector>
        connector = std::make_shared<SSLConnector>(cert_, key_, 0);

    int ret = ErpcHandler().ClientTCPRequest(request, response, connector, ip.c_str(), port);
    iAssert(ret, ("ClientRPCRequest"));

    return 0;
}
