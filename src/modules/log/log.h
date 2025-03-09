#ifndef _LOG
#define _LOG

#include "../../defines.h"
#include <stdarg.h>

typedef enum { INFO, ERROR, WARN } log_level;

log_level get_min_log_level();
// when calling this function, DO NOT FORGET TO ADD A NULL INTO THE LAST
// ARGUMENT, AFTER YOURS STRINGS, OK?
void log(log_level level, int status_code, const char *str, ...);

#endif
