#include "tlog.h"

enum TYPE{
    TYPEA,
    TYPEB,
};

#define XX(a, b) \
    if (a == TYPEA) {\
        TLOG_DBG(("XXX"));\
    } else if (a == TYPEB) {\
        TLOG_DBG(("yyy"));\
    }

int main() 
{
    TLOG(DEBUG, ("my old is %d", 8));
    TLOG_DBG(("my old is %d", 8));
    TLOG_ERR(("my old is %d", 8));
    XX(TYPEA, (1));
    XX(TYPEB, (2));

}
