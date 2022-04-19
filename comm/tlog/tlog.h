#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <string>

enum LOGLevel{
	DEBUG,
	MSG, 
	WARN,
	ERR,
};

class TLog {
public:
    TLog(int level, int line, std::string file, std::string func):_level(level), _line(line), _file(file), _func(func) {}
    void msg_tlog(const char *fmt, ...);
    void tlog(const char *msg);

private:
    int _level;
    int _line;
    std::string _func;
    std::string _file;
};

// TLOG_DBG(("%d", id));
#define TLOG(Level, Wrap)                                \
    do {                                                 \
        TLog tlog(Level, __LINE__, __FILE__, __func__);  \
        tlog.msg_tlog Wrap;                              \
    } while(0)

#define TLOG_DBG(Wrap)                          \
    TLOG(DEBUG, Wrap)

#define TLOG_ERR(Wrap)                          \
    TLOG(ERR, Wrap)

#define TLOG_MSG(Wrap)                          \
    TLOG(MSG, Wrap)

#define TLOG_WARN(Wrap)                         \
    TLOG(WARN, Wrap)

#define iAssert(ret, Wrap)                      \
    do {                                        \
        if (ret < 0) {                          \
            TLOG_ERR(Wrap);                     \
            return -1;                          \
        }                                       \
    } while(0)

#define iAssertNoRet(ret, Wrap)             \
do {                                        \
    if (ret < 0) {                          \
        TLOG_ERR(Wrap);                     \
    }                                       \
} while(0)


#define iAssertRetNull(ret, Wrap)                      \
do {                                        \
    if (ret < 0) {                          \
        TLOG_ERR(Wrap);                     \
        return;                          \
    }                                       \
} while(0)

#define iAssertNull(ret, Wrap)                  \
    do {                                        \
        if (ret == NULL) {                      \
            TLOG_ERR(Wrap);                     \
            return -1;                          \
        }                                       \
    } while(0)


#define TJ      \
    TLOG_DBG(("x"));
