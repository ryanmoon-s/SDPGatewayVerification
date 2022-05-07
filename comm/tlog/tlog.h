#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <string>

// 日志级别开关，只打到此级别
#define LOG_LEVEL_SWITCH 4

#define LOG_BUFF_SIZE 1024

enum LOGLevel{
	ERR = 1,
	WARN = 2,
	MSG = 3,
	DEBUG = 4,
};

class TLog {
public:
    TLog(int level, int line, std::string file, std::string func):level_(level), line_(line), file_(file), func_(func) {}

    void LOGOUT(const char *fmt, ...);

private:

    void _TLOGPRINT(const char *msg);

private:
    int level_;
    int line_;
    std::string file_;
    std::string func_;
};

// TLOG

// TLOG(DEBUG, ("%d", id));
#define TLOG(Level, Wrap)                                   \
    do {                                                    \
        if (Level > LOG_LEVEL_SWITCH)                       \
        {                                                   \
            break;                                          \
        }                                                   \
        TLog tlog(Level, __LINE__, __FILE__, __FUNCTION__); \
        tlog.LOGOUT Wrap;                                   \
    } while(0)

#define TLOG_DBG(Wrap)                          \
    TLOG(DEBUG, Wrap)

#define TLOG_ERR(Wrap)                          \
    TLOG(ERR, Wrap)

#define TLOG_MSG(Wrap)                          \
    TLOG(MSG, Wrap)

#define TLOG_WARN(Wrap)                         \
    TLOG(WARN, Wrap)

// Assert

#define iAssert(ret, Wrap)                      \
    do {                                        \
        if (ret < 0) {                          \
            TLOG_ERR(Wrap);                     \
            return -1;                          \
        }                                       \
    } while(0)

#define iAssertNoReturn(ret, Wrap)          \
do {                                        \
    if (ret < 0) {                          \
        TLOG_ERR(Wrap);                     \
    }                                       \
} while(0)


#define iAssertVoidReturn(ret, Wrap)        \
do {                                        \
    if (ret < 0) {                          \
        TLOG_ERR(Wrap);                     \
        return;                             \
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
    TLOG_DBG((" XXXX XXXX XXXX XXXX XXXX "));
