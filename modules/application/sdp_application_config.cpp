#include "sdp_application_config.h"
#include "comm/tlog/tlog.h"
#include "comm/erpc/erpc_client.h"
#include "comm/commdef/comm_tool.h"

using namespace commtool;

SDPApplicationConfig* SDPApplicationConfig::instance_ = new SDPApplicationConfig();

SDPApplicationConfig::SDPApplicationConfig()
{
    service_ = new SDPApplicationErpcServiceImpl();
}

SDPApplicationConfig::~SDPApplicationConfig()
{
    delete service_;
}

SDPApplicationErpcServiceImpl* SDPApplicationConfig::GetServiceObj()
{
    return service_;
}

int SDPApplicationConfig::FrequencyJudgment(const string& ip)
{
    int ret = 0;
    const time_t min_now = TimeNow_Min();
    auto iter = visist_freq_map_.find(ip);
    if (iter != visist_freq_map_.end())
    {
        Freq& freq = iter->second;
        if (freq.min == min_now)
        {
            freq.times++;
            TLOG_DBG(("TIMES:%d", freq.times));
            if (freq.times > ACCESS_TIMES_LIMIT_ONE_MIN)
            {
                // 重新授权后，一分钟内仍不能访问
                // NOTICE GATEWAY
                erpc::GateFuncNoticeReq req;
                erpc::GateFuncNoticeRsp rsp;
                erpc::Header header;
                req.set_op(erpc::APP_NOTICE_CLOSE_PORT_FOR_IP);
                req.set_op_ip(ip);
                ret = ErpcClient(SSL_CRT_APPLICATION, SSL_KEY_APPLICATION).GateFuncNoticeRequest(req, rsp, header);
                if (ret < 0)
                {
                    TLOG_WARN(("Too freq, notice gateway faild"));
                }
                TLOG_WARN((" ~~~ TOO FREQ, NOTICE GATEWAY ~~~ "));
                return -1;
            }
            return 0;
        }
    }
    visist_freq_map_[ip].min = min_now;
    visist_freq_map_[ip].times = 1;

    TLOG_DBG(("min:%d, times:%d", visist_freq_map_[ip].min, visist_freq_map_[ip].times));
    return 0;
}

int SDPApplicationConfig::TimeLimitJudgement(const string& ip)
{
    int ret = 0;
    
    auto iter = last_visit_time_map_.find(ip);
    if (iter != last_visit_time_map_.end())
    {
        time_t last_time = iter->second;
        if (last_time + ACCESS_TOO_LAST_LIMIT < time(NULL))
        {
            // 重新授权后可继续访问
            iter->second = time(NULL);
            // NOTICE GATEWAY
            erpc::GateFuncNoticeReq req;
            erpc::GateFuncNoticeRsp rsp;
            erpc::Header header;
            req.set_op(erpc::APP_NOTICE_CLOSE_PORT_FOR_IP);
            req.set_op_ip(ip);
            ret = ErpcClient(SSL_CRT_APPLICATION, SSL_KEY_APPLICATION).GateFuncNoticeRequest(req, rsp, header);
            if (ret < 0)
            {
                TLOG_WARN(("GateFuncNoticeRequest faild"));
            }
            TLOG_WARN((" ~~~ LONG TIME NO VISIT, NOTICE GATEWAY ~~~ "));
            return -1;
        }
    }

    last_visit_time_map_[ip] = time(NULL);
    TLOG_DBG(("last visit time:%d", last_visit_time_map_[ip]));

    return 0;
}
