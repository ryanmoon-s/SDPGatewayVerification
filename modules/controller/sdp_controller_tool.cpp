#include "sdp_controller_tool.h"
#include "comm/erpc/erpc_client.h"
#include "comm/commdef/comm_tool.h"
#include "comm/iptools/iptables_tool.h"

int SDPControllerTool::AddUserBlackList(int op, const std::string& ip, int port)
{
    int ret = 0;
    erpc::GateFuncBlackListOpReq req;
    erpc::GateFuncBlackListOpRsp rsp;
    erpc::Header header;

    req.set_op(op);
    req.set_ip(ip);
    req.set_port(port);

    ret = ErpcClient(SSL_CRT_CONTROLLER, SSL_KEY_CONTROLLER).GateFuncBlackListOpRequest(req, rsp, header);
    iAssert(ret, ("GateFuncBlackListOpRequest faild"));

    return 0;
}
