#pragma once

#include <map>
#include <string>
#include <vector>
#include <list>
#include "comm/erpc/erpc_config.h"
#include "comm/iptools/iptables_tool.h"
#include "sdp_controller_service_impl.h"

using namespace std;

class SDPControllerErpcServiceImpl;

enum PerssionMapOp {
    PERSSION_ADD = 1,
    PERSSION_DEL = 2,
};

class SDPControllerConfig {
public:
    IPWhiteList* GetWhiteListObj();
    SDPControllerErpcServiceImpl* GetServiceObj();

    void set_listen_info(const string& ip, int tcp_port, int udp_port);
    int get_tcp_port();

    int QueryAndInsertMD5(const string& md5);
    
    // 管理应用
    void RegisterApp(const string& ip, const vector<erpc::AppItem>& app_list);
    map<string, vector<erpc::AppItem>>* GetAppMap();

    // 管理权限
    int UpdateUserPerssion_Add(const string& account, const string& dst);
    int UpdateUserPerssion_Del(const string& account, const string& dst);
    // 账号密码是否正确
    int CheckUserPasswd(const string& account, const string& passwd);
    // 是否有可访问的列表 同时校验账号密码
    int CheckUserPermission_Exists(const string& account);
    // 是否允许访问此应用
    int CheckUserPermission_Allowed(const string& account, const string& dst);

private:
    IPWhiteList* whitelist_;
    SDPControllerErpcServiceImpl* service_;

    string listen_ip_;
    int tcp_port_;
    int udp_port_;

    // md5去重，防重放攻击
    map<string, int> md5_map_;

    // 应用管理 gateway ip -> app_list
    map<string, vector<erpc::AppItem>> app_map_;
    // 用户可访问列表 account  ->  <服务器IP:服务器端口, 1>
    map<string, map<string, int>> permission_map_;

    // 用户账号、密码
    map<string, string> user_account_map_;

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
