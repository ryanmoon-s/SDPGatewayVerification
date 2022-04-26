#include "sdp_application_service_impl.h"
#include "sdp_application_config.h"

int SDPApplicationErpcServiceImpl::AppFuncHttps(const std::string& request, std::string& response, const erpc::Extra& extra)
{
    int ret = 0;
    auto config = SDPApplicationConfig::GetInstance();
    std::string from_ip = extra.socket_info.ip;
    
    // 频率限制
    ret = config->FrequencyJudgment(from_ip);
    if (ret < 0)
    {
        TLOG_WARN((" ~~~ %s visist too frequently, stop it ~~~ ", from_ip.c_str()));
        return -1;
    }

    // 超时限制
    ret = config->TimeLimitJudgement(from_ip);
    if (ret < 0)
    {
        TLOG_WARN((" ~~~ %s long time no visit , stop it ~~~ ", from_ip.c_str()));
        return -1;
    }

    // 转发业务
    return HTTPS_HANDLER(request, response);
}

int SDPApplicationErpcServiceImpl::HTTPS_HANDLER(const std::string& request, std::string& response)
{
    TLOG_MSG(("HTTPS Get Request:%s", request.c_str()));

    response = "<html><head></head><body>hello world!</body></html>";

    TLOG_MSG(("HTTPS Send Response:%s", response.c_str()));

    return 0;
}
