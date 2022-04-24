#include "comm/erpc/erpc_client.h"
#include "comm/proto/spa.pb.h"
#include "comm/proto/erpc.pb.h"
#include "comm/commdef/comm_tool.h"

#include "sdp_controller_service_impl.h"
#include "sdp_controller_tool.h"
#include "sdp_controller_config.h"

using namespace commtool;
#define GET_RANDOM (rand()%1000000000+1000000000)

int SDPControllerErpcServiceImpl::ConFuncUdpRecv(const std::string& msg, std::string ip, int port)
{
    TLOG_MSG(("GateFuncUdpRecv begin size:%d", msg.size()));
    int ret = 0;
    spa::SPAVoucherPacket spaVoucherPacket;
    spa::SPAVoucher spaVoucher;
    auto config = SDPControllerConfig::GetInstance();

    // 1、解出 spaVoucher
    // 2、验证md5是否匹配(未被修改)
    spaVoucherPacket.ParseFromString(msg);
    ret = SPATools().DecryptVoucher(spaVoucher, spaVoucherPacket, RSA_PRI_KEY_CONTROLLER);
    iAssert(ret, ("DecryptVoucher faild"));
    DBG_PROTO(spaVoucher);

    // TODO iAssert
    ret = SDPControllerTool().CheckUserPermissions(spaVoucher);
    if (ret != 0)
    {
        TLOG_MSG(("CheckUserPermissions faild, spaVoucher data:"));
        MSG_PROTO(spaVoucher);
        return -1;
    }

    // 防止重放攻击
    int repeat = config->QueryAndInsertMD5(spaVoucherPacket.md5_data());
    if (repeat != 0)
    {
        TLOG_MSG(("QueryAndInsertMD5 repeat md5"));
        return -1;
    }

    // 加入白名单
    config->GetWhiteListObj()->OpWhiteList(IP_WHITE_LIST_ADD, ip, port);
    
    return 0;
}

int SDPControllerErpcServiceImpl::ConFuncGetAccess(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp, const erpc::Extra& extra)
{
    MSG_PROTO(objReq);

    int ret = 0;
    erpc::SocketInfo socket_info = extra.socket_info;
    auto config = SDPControllerConfig::GetInstance();

    // 添加 Access List
    // 目前全添加 
    // TODO 根据可访问List添加
    auto app_map = config->GetAppMap();
    for (auto app : *app_map)
    {
        std::string ip = app.first;
        auto app_vec = app.second;
        for (int i = 0; i < app_vec.size(); i++)
        {
            erpc::AccessItem* item = objRsp.add_access_list();
            item->set_ip(ip);
            item->mutable_app()->CopyFrom(app_vec[i]);

            // Ticket 构造
            // 一个item -> 一个app -> 一个ticket
            spa::SPATicket spaTicket;
            spaTicket.set_ip(socket_info.ip.c_str());
            spaTicket.set_timestamp(time(NULL));
            spaTicket.set_random(GET_RANDOM);
            spaTicket.set_valid_seconds(30);

            // Ticket 签名
            spa::SPATicketPacket spaTicketPacket;
            ret = SPATools().SignTicket(spaTicketPacket, spaTicket, RSA_PRI_KEY_CONTROLLER);
            iAssert(ret, ("EncryptVoucher faild"));

            item->mutable_ticket_packet()->CopyFrom(spaTicketPacket);
        }
    }

    MSG_PROTO(objRsp);
    return 0;
}

int SDPControllerErpcServiceImpl::ConFuncRegisterApp(const erpc::ConFuncRegisterAppReq& objReq, erpc::ConFuncRegisterAppRsp& objRsp, const erpc::Extra& extra)
{
    MSG_PROTO(objReq);

    if (objReq.app_list_size() == 0)
    {
        return -1;
    }

    int ret = 0;
    erpc::SocketInfo socket_info = extra.socket_info;
    std::string ip = socket_info.ip;
    auto config = SDPControllerConfig::GetInstance();

    std::vector<erpc::AppItem> app_vec;
    for (int i = 0; i < objReq.app_list_size(); i++)
    {
        app_vec.push_back(objReq.app_list(i));
    }

    config->RegisterApp(ip, app_vec);

    MSG_PROTO(objRsp);
    return 0;
}
