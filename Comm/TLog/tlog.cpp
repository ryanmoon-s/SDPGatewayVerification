#include "tlog.h"

std::string log_level[] = { "DEBUG", "MSGXX", "WARNX", "ERROR", "NODEF"};

void TLog::tlog(LOGLevel level, const char *msg) 
{
	time_t t;
	time(&t);
	char time_str[20];
    std::string prefix = log_level[level];

	strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&t));	
    fprintf(stdout, "[%s] [%s] [%s:%d %s] %s\n", time_str, prefix.c_str(), this->_file.c_str(), this->_line, this->_func.c_str(), msg);
}

void TLog::msg_tlog(LOGLevel level, const char *fmt, ...)
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
	
	tlog(level, buf);
}


