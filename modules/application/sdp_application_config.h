#pragma once

#include <map>
#include <string>
#include <vector>
#include "comm/erpc/erpc_config.h"
#include "sdp_application_service_impl.h"

// 超过时限，未进行访问，则
#define ACCESS_TIME_LIMIT_ONE_MIN   3

using namespace std;

class SDPApplicationErpcServiceImpl;

class SDPApplicationConfig {
public:
    SDPApplicationErpcServiceImpl* GetServiceObj();

    int FrequencyJudgment(const string& ip);

private:
    SDPApplicationErpcServiceImpl* service_;
    
    // 源IP 一分钟内的访问次数
    struct Freq {
        int times = 0;
        int min = 0;
    };
    map<string, Freq> visit_map_;


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
