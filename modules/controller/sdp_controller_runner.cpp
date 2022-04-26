#include "sdp_controller.h"
#include "comm/erpc/erpc_client.h"
#include "comm/commdef/comm_tool.h"
#include "comm/iptools/iptables_tool.h"
#include <thread>

using namespace std;

void scanning_func();

int main()
{
    thread th(scanning_func);
    th.detach();

    SDPController controller;
    controller.Run();

    return 0;
}

void scanning_func()
{
    TLOG_MSG(("controller scanning thread begin to run ..."));
    while (true)
    {
        // 模拟：定期扫描不安全的客户端，加入网关黑名单
        this_thread::sleep_for(chrono::seconds(30));

        int ret = 0;
        erpc::GateFuncBlackListOpReq req;
        erpc::GateFuncBlackListOpRsp rsp;
        erpc::Header header;

        req.set_op(IP_BLACK_LIST_ADD);
        req.set_ip(IP_CLIENT_PB);
        req.set_port(TCP_PORT_APPLICATION);

        ret = ErpcClient(SSL_CRT_CONTROLLER, SSL_KEY_CONTROLLER).GateFuncBlackListOpRequest(req, rsp, header);
        if (ret < 0)
        {
            TLOG_MSG(("GateFuncBlackListOpRequest faild"));
        }
        TLOG_MSG((" ************************* ADD BLACK ************************* "));
        MSG_PROTO(req);

        // 去除黑名单
        this_thread::sleep_for(chrono::seconds(30));

        req.set_op(IP_BLACK_LIST_DEL);
        ret = ErpcClient(SSL_CRT_CONTROLLER, SSL_KEY_CONTROLLER).GateFuncBlackListOpRequest(req, rsp, header);
        if (ret < 0)
        {
            TLOG_MSG(("GateFuncBlackListOpRequest faild"));
        }
        TLOG_MSG((" ************************* DEL BLACK ************************* "));
        MSG_PROTO(req);
        
    }
}
