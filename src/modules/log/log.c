#include "log.h"
#include "stdio.h"

extern log_level min_log_level;
extern bool debug;

log_level get_min_log_level(void) {
  if (debug)
    return INFO;
  return WARN;
}

void c_log(log_level level, status_code status, const char *str, ...) {
  if (level < min_log_level)
    return;

  switch (level) {
  case ERROR:
    puts("\n/--------ERROR--------/");
    break;
  case WARN:
    puts("\n/--------WARN--------/");
    break;
  case INFO:
    puts("\n/--------INFO--------/");
    break;
  case DEBUG:
    puts("\n/--------DEBUG--------/");
    break;
  case CRIT_ERROR:
    puts("\n/--------CRIT_ERROR--------/");
    break;
  default:
    break;
  }

  if ((status / 100) != -1) {
    printf("STATUS CODE: ");
  }
  switch (status / 100) {
  case -1:
    break;
  case 0:
    printf("%d - SCHEDULER\n", status);
    break;
  case 1:
    printf("%d - Process Handling Context\n", status);
    break;
  case 2:
    printf("%d - CPU Related Context\n", status);
    break;
  case 3:
    printf("%d - Memory Status Context\n", status);
    break;
  case 5:
    printf("%d - User Status Context\n", status);
    break;
  default:
    printf("%d - Unknown Status\n", status);
  }

  va_list arg;
  va_start(arg, str);

  while (str) {
    printf("-> %s\n", str);
    str = va_arg(arg, const char *);
  }

  va_end(arg);
  puts("");
}
