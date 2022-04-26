#pragma once

#include <map>
#include <string>
#include <vector>
#include "comm/erpc/erpc_config.h"
#include "sdp_application_service_impl.h"

// 超过时限，未进行访问 2小时
#define ACCESS_TOO_LAST_LIMIT   10

// 一分钟内限频 3次
#define ACCESS_TIMES_LIMIT_ONE_MIN   3

using namespace std;

class SDPApplicationErpcServiceImpl;

class SDPApplicationConfig {
public:
    SDPApplicationErpcServiceImpl* GetServiceObj();

    int FrequencyJudgment(const string& ip);
    int TimeLimitJudgement(const string& ip);

private:
    SDPApplicationErpcServiceImpl* service_;
    
    // 源IP 一分钟内的访问次数超限，不处理请求，并通知Gateway去除允许规则
    struct Freq {
        int times = 0;
        int min = 0;
    };
    map<string, Freq> visist_freq_map_;

    // 上一次请求，距离现在太久，不处理请求，并通知Gateway去除允许规则
    // 懒更新
    map<string, time_t> last_visit_time_map_;

public:
    static SDPApplicationConfig* GetInstance() 
    {
        return instance_;
    }
    ~SDPApplicationConfig();

private:
    SDPApplicationConfig();
    SDPApplicationConfig(const SDPApplicationConfig&){};

    static SDPApplicationConfig* instance_;
};
