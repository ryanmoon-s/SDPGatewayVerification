#pragma once

#include <map>
#include <string>
#include <vector>
#include <list>
#include "comm/erpc/erpc_config.h"
#include "comm/iptools/iptables_tool.h"
#include "sdp_controller_service_impl.h"

class SDPControllerErpcServiceImpl;

enum PerssionMapOp {
    PERSSION_ADD = 1,
    PERSSION_DEL = 2,
};

class SDPControllerConfig {
public:
    IPWhiteList* GetWhiteListObj();
    SDPControllerErpcServiceImpl* GetServiceObj();

    void set_listen_info(const std::string& ip, int tcp_port, int udp_port);
    int get_tcp_port();

    int QueryAndInsertMD5(const std::string& md5);
    
    // 管理应用
    void RegisterApp(const std::string& ip, const std::vector<erpc::AppItem>& app_list);
    std::map<std::string, std::vector<erpc::AppItem>>* GetAppMap();

    // 管理权限
    int UpdatePerssionMap_Add(const std::string& account, const std::string& dst);
    int UpdatePerssionMap_Del(const std::string& account, const std::string& dst);
    int CheckUserPermission(const std::string& account, const std::string& dst);

private:
    IPWhiteList* whitelist_;
    SDPControllerErpcServiceImpl* service_;

    std::string listen_ip_;
    int tcp_port_;
    int udp_port_;

    // md5去重，防重放攻击
    std::map<std::string, int> md5_map_;

    // 应用管理 gateway ip -> app_list
    std::map<std::string, std::vector<erpc::AppItem>> app_map_;
    // 用户可访问列表 user account -> app -> 127.0.0.1:80
    std::map<std::string, std::list<std::string>> permission_map_;

public:
    static SDPControllerConfig* GetInstance() 
    {
        return instance_;
    }
    ~SDPControllerConfig();

private:
    SDPControllerConfig();
    SDPControllerConfig(const SDPControllerConfig&){};

    static SDPControllerConfig* instance_;
};
