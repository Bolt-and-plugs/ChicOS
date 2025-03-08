#ifndef _LOG
#define _LOG

#include <stdarg.h>

typedef enum { ERROR = -1, WARN = 0, INFO = 1 } log_status;

// when calling this function, DO NOT FORGET TO ADD A NULL INTO THE LAST ARGUMENT, AFTER YOURS STRINGS, OK?
void log(log_status level, int status_code, const char *str , ...);

#endif
