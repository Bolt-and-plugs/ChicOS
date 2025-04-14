#ifndef _LOG
#define _LOG

#include "../../defines.h"
#include <stdarg.h>

typedef enum { DEBUG, INFO, WARN, ERROR, CRIT_ERROR } log_level;

// this is just a todo status code, in order to make things easy to debug
// Im gonna make this prefix reffers to a system part
// yet to change
typedef enum {
  SCHEDDULER_THING = 000,
  PROCESS_OUT_OF_LIST = 101,
  CPU_THING = 200,
  MEM_STATUS = 300,
  MEM_ERROR = 301,
  USER_STATUS = 500,
  DISK_ERROR = 400,
  DISK_FILE_READ_ERROR = 401,
  DISK_FILE_WRITE_ERROR = 402,
  SEMAPHORE_ERROR = 500,
  SEMAPHORE_INIT_ERROR = 501,
  SEMAPHORE_POST_ERROR = 502,
  SEMAPHORE_WAIT_ERROR = 503
} status_code;

log_level get_min_log_level();
// when calling this function, DO NOT FORGET TO ADD A NULL INTO THE LAST
// ARGUMENT, AFTER YOURS STRINGS, OK?
// example c_log(INFO, 0, "Message I want to pass", NULL);
void c_log(log_level level, status_code status_code, const char *str, ...);

#endif
