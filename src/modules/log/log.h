#ifndef _LOG
#define _LOG

#include "../../defines.h"
#include <stdarg.h>

typedef enum { DEBUG, INFO, ERROR, WARN } log_level;

// this is just a todo status code, in order to make things easy to debug
// Im gonna make this prefix reffers to a system part
// yet to change
typedef enum {
  SCHEDDULER_THING = 000,
  PROCESS_OUT_OF_LIST = 101,
  CPU_THING = 200,
  MEM_STATUS = 300,
  USER_STATUS = 500,
} status_code;

log_level get_min_log_level();
// when calling this function, DO NOT FORGET TO ADD A NULL INTO THE LAST
// ARGUMENT, AFTER YOURS STRINGS, OK?
// example c_log(INFO, 0, "Message I want to pass", NULL);
void c_log(log_level level, status_code status_code, const char *str, ...);

#endif
