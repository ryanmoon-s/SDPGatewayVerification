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

    auto iter = visit_map_.find(ip);
    if (iter != visit_map_.end())
    {
        Freq& freq = iter->second;
        if (freq.min == min_now)
        {
            if (++freq.times > ACCESS_TIME_LIMIT_ONE_MIN)
            {
                // NOTICE GATEWAY
                erpc::GateFuncNoticeReq req;
                erpc::GateFuncNoticeRsp rsp;
                erpc::Header header;
                req.set_op(erpc::APP_NOTICE_CLOSE_PORT_FOR_IP);
                ret = ErpcClient(SSL_CRT_APPLICATION, SSL_KEY_APPLICATION).GateFuncNoticeRequest(req, rsp, header);
                if (ret < 0)
                {
                    TLOG_WARN(("Too freq, notice gateway faild"));
                }
                return -1;
            }
            return 0;
        }
    }
    
    visit_map_[ip].min = min_now;
    visit_map_[ip].times = 0;

    TLOG_DBG(("min:%d, times:%d", visit_map_[ip].min, visit_map_[ip].times));
    return 0;
}
