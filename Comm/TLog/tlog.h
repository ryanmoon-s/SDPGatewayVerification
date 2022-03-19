#ifndef LOG_H
#define LOG_H

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
    TLog(int line, std::string file, std::string func):_line(line), _file(file), _func(func) {}
    void msg_tlog(LOGLevel level, const char *fmt, ...);
    void tlog(LOGLevel level, const char *msg);

private:
    int _line;
    std::string _func;
    std::string _file;
};

// TLOG((DEBUG, "%d", id));
#define TLOG(X)	                                  \
    do {                                          \
        TLog tlog(__LINE__, __FILE__, __func__);  \
        tlog.msg_tlog X;                          \
    } while(0)

#define iAssert(ret, Func)                      \
    do {                                        \
        if (ret < 0) {                          \
            TLOG((ERR, "Func error"));         \
            return -1;                          \
        }                                       \
    } while(0)


#endif

