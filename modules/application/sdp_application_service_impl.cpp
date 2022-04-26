#include "sdp_application_service_impl.h"
#include "sdp_application_config.h"

int SDPApplicationErpcServiceImpl::AppFuncHttps(const std::string& request, std::string& response, const erpc::Extra& extra)
{
    // 频率限制
    int ret = 0;
    auto config = SDPApplicationConfig::GetInstance();
    std::string from_ip = extra.socket_info.ip;
    
    ret = config->FrequencyJudgment(from_ip);
    if (ret < 0)
    {
        TLOG_WARN((" ~~~ %s visist too frequently, stop it ~~~ "));
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
