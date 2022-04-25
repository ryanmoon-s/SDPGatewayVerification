#include "sdp_access_client.h"

int AccessClient::AccessApplication_HTTPS(const std::vector<erpc::AccessItem>& list) 
{
    int ret = 0;
    int has_https = -1;
    erpc::AccessItem item;

    // 查找是否有HTTPS应用
    for (auto it : list)
    {
        std::string appname = it.app().appname();
        if (appname == "HTTPS")
        {
            item = it;
            has_https = 1;
            break;
        }
    }
    iAssert(has_https, ("No HTTPS Application"));

    // 访问应用
    std::string ip = item.ip();
    int app_tcp_port = item.app().app_tcp_port();
    std::string request("https request");
    std::string response;

    ret = erpc_client_.AppFuncHttpsRequest(request, response, ip, app_tcp_port);
    iAssert(ret, ("AppFuncHttpsRequest faild"));

    TLOG_MSG(("HTTPS Response:"));
    TLOG_MSG(("%s", response.c_str()));

    return 0;
}
