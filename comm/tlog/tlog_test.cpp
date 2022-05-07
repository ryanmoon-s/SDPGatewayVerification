#include "tlog.h"

int main() 
{
    TLOG(DEBUG, ("my age is %d", 8));
    TLOG_DBG(("my age is %d", 8));
    TLOG_ERR(("my age is %d", 8));
    TLOG_WARN(("my age is %d", 8));
    TLOG_MSG(("my age is %d", 8));
    TLOG_MSG((""));

    TLOG_DBG(("i'm %d years old.", 20));
}
