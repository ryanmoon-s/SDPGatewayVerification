#include "sdp_application_config.h"

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
