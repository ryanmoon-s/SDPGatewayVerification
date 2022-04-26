#include "sdp_application_service_impl.h"

int SDPApplicationErpcServiceImpl::AppFuncHttps(const std::string& request, std::string& response)
{
    TLOG_MSG(("HTTPS Get Request:%s", request.c_str()));
    
    response = "<html><head></head><body>hello world!</body></html>";

    TLOG_MSG(("HTTPS Send Response:%s", response.c_str()));
    return 0;
}
