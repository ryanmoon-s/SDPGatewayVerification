#include "tlog.h"

static const char *log_level[] = {" ", "ERROR", "WARN-", "MSG--", "DEBUG", "NODEF"};

void TLog::tlog(const char *msg) 
{
	time_t t;
	time(&t);
	char time_str[20];
    const char *prefix = log_level[_level];

	strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&t));	
    fprintf(stdout, "[%s] [%s] [%s:%d %s] %s\n", time_str, prefix, _file.c_str(), _line, _func.c_str(), msg);
}

void TLog::msg_tlog(const char *fmt, ...)
{
    char buf[1024];
	va_list ap;

    va_start(ap, fmt);
	if (fmt != NULL) 
    {
        vsnprintf(buf, sizeof(buf), fmt, ap);
    }
    else 
    {
        buf[0] = '\0';
    }
    va_end(ap);
	
	tlog(buf);
}


