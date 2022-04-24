#include "sdp_controller_config.h"

SDPControllerConfig* SDPControllerConfig::instance_ = new SDPControllerConfig();

SDPControllerConfig::SDPControllerConfig()
{
    whitelist_ = new IPWhiteList();
    service_ = new SDPControllerErpcServiceImpl();
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

void SDPControllerConfig::set_listen_info(const std::string& ip, int tcp_port, int udp_port)
{
    listen_ip_ = ip;
    tcp_port_ = tcp_port;
    udp_port_ = udp_port;
}

int SDPControllerConfig::get_tcp_port()
{
    return tcp_port_;
}

int SDPControllerConfig::QueryAndInsertMD5(const std::string& md5)
{
    auto iter = md5_map_.find(md5);
    if (iter != md5_map_.end())
    {
        // 找到
        return 1;
    }
    else
    {
        // 没找到
        md5_map_.insert(std::make_pair(md5, 1));
        return 0;
    }
    return 0;
}

void SDPControllerConfig::RegisterApp(const std::string& ip, const std::vector<erpc::AppItem>& app_list)
{
    if (!app_list.empty())
    {
        app_map_[ip] = app_list;
    }
}

std::map<std::string, std::vector<erpc::AppItem>>* SDPControllerConfig::GetAppMap()
{
    return &app_map_;
}

int SDPControllerConfig::UpdatePerssionMap_Add(const std::string& account, const std::string& dst)
{
    auto iter = permission_map_.find(account);

    // 已经存在的情况下，不更新
    if (iter != permission_map_.end())
    {
        auto& m_list = iter->second;
        for (auto it = m_list.begin(); it != m_list.end(); it++)
        {
            if (*it == dst)
            {
                // 已经存在，未更新
                return -1;
            }
        }
    }

    // 否则，更新
    permission_map_[account].push_back(dst);
    return 0;
}

int SDPControllerConfig::UpdatePerssionMap_Del(const std::string& account, const std::string& dst)
{
    auto iter = permission_map_.find(account);
    if (iter != permission_map_.end())
    {

    }
}

int SDPControllerConfig::CheckUserPermission(const std::string& account, const std::string& dst)
{
    auto iter = permission_map_.find(account);
    if (iter != permission_map_.end())
    {

    }
}