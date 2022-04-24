#include "sdp_application_service_impl.h"

int SDPApplicationErpcServiceImpl::AppFuncHttps(const std::string& request, std::string& response)
{
    TLOG_MSG(("request:%s", request.c_str()));
    response = "<html><head></head><body>hello world!</body></html>";
    return 0;
}
