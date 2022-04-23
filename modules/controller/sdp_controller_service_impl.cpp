#include "comm/erpc/erpc_client.h"
#include "comm/proto/spa.pb.h"
#include "comm/proto/erpc.pb.h"
#include "comm/commdef/comm_tool.h"

#include "sdp_controller_service_impl.h"
#include "sdp_controller_tool.h"
#include "sdp_controller_config.h"

using namespace commtool;

int SDPControllerErpcServiceImpl::ConFuncUdpRecv(const std::string& msg, std::string ip, int port)
{
    int ret = 0;
    spa::SPAVoucherPacket spaVoucherPacket;
    spa::SPAVoucher spaVoucher;
    auto config = SDPControllerConfig::GetInstance();

    // 解出 spaVoucher
    spaVoucherPacket.ParseFromString(msg);
    ret = SPATools().DecryptVoucher(spaVoucher, spaVoucherPacket);
    iAssert(ret, ("DecryptVoucher faild"));

    // 输出 spaVoucher 检查日志
    std::string voucher_str;
    spaVoucher.SerializeToString(&voucher_str);
    TLOG_DBG(("spaVoucher: %s", voucher_str.c_str()));

    // TODO iAssert
    ret = SDPControllerTool().CheckUserPermissions(spaVoucher);
    if (ret != 0)
    {
        TLOG_MSG(("CheckUserPermissions faild, spaVoucher data:%s", voucher_str.c_str()));
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
    config->GetWhiteListObj()->OpWhiteList(IP_WHITE_TABLE_ADD, ip, port);
    
    return 0;
}

int SDPControllerErpcServiceImpl::ConFuncGetAccess(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp, const erpc::Extra& extra)
{
    TLOG_PROTO(objReq);

    int ret = 0;
    erpc::SocketInfo socket_info = extra.socket_info;
    auto config = SDPControllerConfig::GetInstance();

    // 派发 ticket
    spa::SPATicket spaTicket;
    spaTicket.set_ip(socket_info.ip.c_str());
    spaTicket.set_timestamp(time(NULL));

    spa::SPATicketPacket spaTicketPacket;
    ret = SPATools().EncryptVoucher(spaTicketPacket, spaTicket);
    iAssert(ret, ("EncryptVoucher faild"));

    objRsp.mutable_ticket_packet()->CopyFrom(spaTicketPacket);

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
        }
    }

    objRsp.mutable_ticket_packet()->CopyFrom(spaTicketPacket);

    TLOG_PROTO(objRsp);
    return 0;
}

int SDPControllerErpcServiceImpl::ConFuncRegisterApp(const erpc::ConFuncRegisterAppReq& objReq, erpc::ConFuncRegisterAppRsp& objRsp, const erpc::Extra& extra)
{
    TLOG_PROTO(objReq);

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

    TLOG_PROTO(objRsp);
    return 0;
}
