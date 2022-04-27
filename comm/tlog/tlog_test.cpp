#include "tlog.h"

int main() 
{
    TLOG(DEBUG, ("my old is %d", 8));
    TLOG_DBG(("my old is %d", 8));
    TLOG_ERR(("my old is %d", 8));
    TLOG_WARN(("my old is %d", 8));
    TLOG_MSG(("my old is %d", 8));
    TLOG_MSG((""));
}
