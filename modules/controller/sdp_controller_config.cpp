#include "sdp_controller_config.h"

SDPControllerConfig* SDPControllerConfig::instance_ = new SDPControllerConfig();

SDPControllerConfig::SDPControllerConfig()
{
    // 初始化白名单、服务列表
    whitelist_ = new IPWhiteList();
    service_ = new SDPControllerErpcServiceImpl();

    // 初始化用户账号
    user_account_map_.insert(make_pair("xiaoming", "123456"));
    user_account_map_.insert(make_pair("xiaohong", "123456"));
    user_account_map_.insert(make_pair("xiaobai", "123456"));
}

SDPControllerConfig::~SDPControllerConfig()
{
    delete whitelist_;
    delete service_;
}

IPWhiteList* SDPControllerConfig::GetWhiteListObj()
{
    return whitelist_;
}

SDPControllerErpcServiceImpl* SDPControllerConfig::GetServiceObj()
{
    return service_;
}

void SDPControllerConfig::set_listen_info(const string& ip, int tcp_port, int udp_port)
{
    listen_ip_ = ip;
    tcp_port_ = tcp_port;
    udp_port_ = udp_port;
}

int SDPControllerConfig::get_tcp_port()
{
    return tcp_port_;
}

int SDPControllerConfig::QueryAndInsertMD5(const string& md5)
{
    auto iter = md5_map_.find(md5);
    if (iter != md5_map_.end())
    {
        TLOG_ERR(("md5 repeated"));
        return -1;
    }
    else
    {
        md5_map_.insert(make_pair(md5, 1));
        return 0;
    }
    return 0;
}

void SDPControllerConfig::RegisterApp(const string& ip, const vector<erpc::AppItem>& app_list)
{
    if (!app_list.empty())
    {
        app_map_[ip] = app_list;
    }
}

map<string, vector<erpc::AppItem>>* SDPControllerConfig::GetAppMap()
{
    return &app_map_;
}

int SDPControllerConfig::UpdateUserPerssion_Add(const string& account, const string& dst)
{
    auto iter = permission_map_.find(account);

    // 已经存在
    if (iter != permission_map_.end())
    {
        auto& dst_map = iter->second;
        auto iter2 = dst_map.find(dst);
        if (iter2 != dst_map.end())
        {
            TLOG_ERR(("dst exists"));
            return 1;
        }
    }

    // 不存在，插入
    permission_map_[account].insert(make_pair(dst, 1));
    return 0;
}

int SDPControllerConfig::UpdateUserPerssion_Del(const string& account, const string& dst)
{
    auto iter = permission_map_.find(account);

    // 存在，删除
    if (iter != permission_map_.end())
    {
        auto& dst_map = iter->second;
        auto iter2 = dst_map.find(dst);
        if (iter2 != dst_map.end())
        {
            dst_map.erase(dst);
            return 0;
        }
    }

    // 不存在
    TLOG_ERR(("dst not exists"));
    return 1;
}

int SDPControllerConfig::CheckUserPasswd(const string& account, const string& passwd)
{
    auto acc_iter = user_account_map_.find(account);
    if (acc_iter != user_account_map_.end())
    {
        if (acc_iter->second != passwd)
        {
            TLOG_ERR(("passwd not match"));
            return -2;
        }
    }
    else
    {
        TLOG_ERR(("account not exists"));
        return -1;
    }

    return 0;
}

int SDPControllerConfig::CheckUserPermission_Exists(const string& account)
{
    auto iter = permission_map_.find(account);
    if (iter != permission_map_.end())
    {
        if (! iter->second.empty())
        {
            return 0;
        }
    }

    TLOG_ERR(("no list to access"));
    return -3;
}

int SDPControllerConfig::CheckUserPermission_Allowed(const string& account, const string& dst)
{
    auto iter = permission_map_.find(account);
    if (iter != permission_map_.end())
    {
        auto& dst_map = iter->second;
        auto iter2 = dst_map.find(dst);
        if (iter2 != dst_map.end())
        {
            return 0;
        }
    }
    
    TLOG_ERR(("not allowed to access this dst"));
    return -1;
}
