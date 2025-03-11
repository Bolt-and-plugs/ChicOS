#include "log.h"
#include "stdio.h"

extern log_level min_log_level;
extern bool debug;

log_level get_min_log_level(void) {
  if (debug)
    return INFO;
  return WARN;
}

void log(log_level level, status_code status, const char *str, ...) {
  if (level < min_log_level)
    return;

  printf("\n/---------------------/\n");
  printf("STATUS CODE: ");
  switch (status / 100) {
  case 0:
    printf("%d - SCHEDULER\n", status);
    break;
  case 1:
    printf("%d - Process Handling Issue\n", status);
    break;
  case 2:
    printf("%d - CPU Related Issue\n", status);
    break;
  case 3:
    printf("%d - Memory Status Issue\n", status);
    break;
  case 5:
    printf("%d - User Status Issue\n", status);
    break;
  default:
    printf("%d - Unknown Status\n", status);
  }

  printf("LEVEL: ");
  switch (level) {
  case ERROR:
    puts("ERROR");
    break;

  case WARN:
    puts("WARN");
    break;
  case INFO:
    puts("INFO");
    break;
  default:
    puts("INFO");
    break;
  }

  va_list arg;
  va_start(arg, str);

  while (str) {
    printf("Argument: %s\n", str);
    str = va_arg(arg, const char *);
  }

  va_end(arg);
  puts("");
}
