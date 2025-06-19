#ifndef _LOG
#define _LOG

#include "../../defines.h"
#include "ncurses.h"
#include <stdarg.h>

typedef enum { DEBUG, INFO, WARN, ERROR, CRIT_ERROR } log_level;

// this is just a todo status code, in order to make things easy to debug
// Im gonna make this prefix reffers to a system part
// yet to change
typedef enum {
  SCHEDULER_PROCESS_OUT_OF_BOUNDS = 000,
  DEFAULT_STATUS = -100,
  PROCESS_STATUS = 100,
  PROCESS_OUT_OF_LIST = 101,
  PROCESS_CREATION_ERROR = 102,
  PROCESS_DELETION_ERROR = 103,
  CPU_ERROR = 200,
  MEM_STATUS = 300,
  MEM_ERROR = 301,
  MEM_FULL = 302,
  MEM_ALLOC_FAIL = 303,
  MEM_DEALLOC_FAIL = 304,
  MEM_REALLOC_FAIL = 305,
  USER_STATUS = 500,
  DISK_ERROR = 400,
  DISK_FILE_READ_ERROR = 401,
  DISK_FILE_WRITE_ERROR = 402,
  DISK_OPEN_ERROR = 403,
  SEMAPHORE_ERROR = 500,
  SEMAPHORE_INIT_ERROR = 501,
  SEMAPHORE_POST_ERROR = 502,
  SEMAPHORE_WAIT_ERROR = 503,
  THREAD_INIT_ERROR = 504,
  INVALID_INPUT = 600,
  QUEUE_EMPTY = 700,
  QUEUE_ERROR = 701,
  USER_HASH_ERROR = 800,

} status_code;

// defining correct macros for logging
#define c_crit_error(s, ...)                                                   \
  c_log(CRIT_ERROR, s, ##__VA_ARGS__, NULL);                                   \
  clear();                                                                     \
  refresh();                                                                   \
  endwin();                                                                    \
  exit(1);
#define c_error(s, ...) c_log(ERROR, s, ##__VA_ARGS__, NULL)
#define c_warn(s, ...) c_log(WARN, s, ##__VA_ARGS__, NULL)
#define c_info(...) c_log(INFO, DEFAULT_STATUS, ##__VA_ARGS__, NULL)
#define c_debug(s, ...) c_log(DEBUG, s, ##__VA_ARGS__, NULL)

log_level get_min_log_level();

// do not use this directly unless you are sure you need to
void c_log(log_level level, status_code status_code, const char *str, ...);

#endif
