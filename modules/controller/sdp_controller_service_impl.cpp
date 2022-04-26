#include "comm/erpc/erpc_client.h"
#include "comm/proto/spa.pb.h"
#include "comm/proto/erpc.pb.h"
#include "comm/commdef/comm_tool.h"

#include "sdp_controller_service_impl.h"
#include "sdp_controller_tool.h"
#include "sdp_controller_config.h"

using namespace commtool;

#define HANDLE_VERIFY_FAILD(LOG, ip, port)      \
    if (ret != 0)                               \
    {                                           \
        config->GetWhiteListObj()->OpWhiteList(IP_WHITE_LIST_DEL, ip, port);    \
        TLOG_MSG((LOG));                                                        \
        return 1;                                                               \
    }

int SDPControllerErpcServiceImpl::ConFuncUdpRecv(const std::string& msg, std::string from_ip, int from_port)
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

    /***************************** 用户鉴权 *****************************/
    
    // 1、账号密码正确
    spa::Account account = spaVoucher.account();
    ret = config->CheckUserPasswd(account.acc(), account.pwd());
    HANDLE_VERIFY_FAILD("CheckUserPasswd faild", from_ip, config->get_tcp_port());

    // 2、有可访问的应用列表
    ret = config->CheckUserPermission_Exists(spaVoucher.account().acc());
    HANDLE_VERIFY_FAILD("CheckUserPermission faild", from_ip, config->get_tcp_port());

    // 3、ip 地址
    // 4、mac 地址
    // 5、address


    // 防止重放攻击
    int repeat = config->QueryAndInsertMD5(spaVoucherPacket.md5_data());
    if (repeat != 0)
    {
        TLOG_MSG(("QueryAndInsertMD5 repeat md5"));
        MSG_PROTO(spaVoucherPacket);
        return 1;
    }

    // 加入本机白名单
    config->GetWhiteListObj()->OpWhiteList(IP_WHITE_LIST_ADD, from_ip, config->get_tcp_port());
    
    TLOG_MSG((" ~~ NOTICE: CONTROLLER SPA VERIFICATION PASSED ~~ "));

    return 0;
}

int SDPControllerErpcServiceImpl::ConFuncGetAccess(const erpc::ConFuncGetAccessReq& objReq, erpc::ConFuncGetAccessRsp& objRsp, const erpc::Extra& extra)
{
    MSG_PROTO(objReq);

    int ret = 0;
    erpc::SocketInfo socket_info = extra.socket_info;
    spa::Account user_account = objReq.account();
    auto config = SDPControllerConfig::GetInstance();

    // 添加 Access List
    auto app_map = config->GetAppMap();
    for (auto app : *app_map)
    {
        std::string gateway_ip = app.first;
        auto app_vec = app.second;
        for (int i = 0; i < app_vec.size(); i++)
        {
            erpc::AppItem app_item = app_vec[i];

            // 用户鉴权
            std::string dst = gateway_ip + ":" + std::to_string(app_item.app_tcp_port());
            ret = config->CheckUserPermission_Allowed(user_account.acc(), dst);  // 用户是否可访问此app
            if (ret != 0)
            {
                TLOG_DBG(("User not allowed to access, skip"));
                TLOG_DBG(("User: acc:%s, dst:%s", user_account.acc().c_str(), dst.c_str()));
                continue;
            }

            // 添加app_item
            erpc::AccessItem* acc_item = objRsp.add_access_list();
            acc_item->set_ip(gateway_ip);
            acc_item->mutable_app()->CopyFrom(app_item);

            // Ticket 构造
            spa::SPATicket spaTicket;
            spaTicket.set_ip(socket_info.ip.c_str());  // 用户ip
            spaTicket.set_timestamp(time(NULL));
            spaTicket.set_random(GET_RANDOM);
            spaTicket.set_valid_seconds(30);

            // Ticket 签名
            spa::SPATicketPacket spaTicketPacket;
            ret = SPATools().SignTicket(spaTicketPacket, spaTicket, RSA_PRI_KEY_CONTROLLER);
            iAssert(ret, ("EncryptVoucher faild"));
            acc_item->mutable_ticket_packet()->CopyFrom(spaTicketPacket);
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
    std::vector<std::string> perssion_vec;
    for (int i = 0; i < objReq.app_list_size(); i++)
    {
        erpc::AppItem item  = objReq.app_list(i);
        app_vec.push_back(item);

        // 权限列表
        std::string dst = ip + ":" + std::to_string(item.app_tcp_port());
        perssion_vec.push_back(dst);
    }

    // 注册到config
    config->RegisterApp(ip, app_vec);

    // 用户授权
    for (int i = 0; i < perssion_vec.size(); i++)
    {
        std::string per = perssion_vec[i];
        config->UpdateUserPerssion_Add("xiaoming", per);
        config->UpdateUserPerssion_Add("xiaohong", per);
        config->UpdateUserPerssion_Add("xiaobai", per);

        TLOG_DBG(("add permission:%s", per.c_str()));
    }

    MSG_PROTO(objRsp);
    return 0;
}
