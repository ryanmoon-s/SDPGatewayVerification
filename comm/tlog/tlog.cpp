#include "tlog.h"

static const char *log_level[] = {" ", "ERROR", "WARN-", "MSG--", "DEBUG", "NODEF"};

void TLog::_TLOGPRINT(const char *msg) 
{
	time_t t = time(NULL);
	char time_str[20];
    const char *prefix = log_level[level_];

	strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&t));	
    fprintf(stdout, "[%s] [%s] [%s:%d %s] %s\n", time_str, prefix, file_.c_str(), line_, func_.c_str(), msg);
}

void TLog::LOGOUT(const char *fmt, ...)
{
    char buf[LOG_BUFF_SIZE];
    buf[0] = '\0';
	va_list ap;

    va_start(ap, fmt);
	if (fmt != NULL) 
    {
        vsnprintf(buf, sizeof(buf), fmt, ap);
    }
    va_end(ap);
	
	_TLOGPRINT(buf);
}
