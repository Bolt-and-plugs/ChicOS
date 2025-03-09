#include "log.h"
#include "stdio.h"

extern log_level min_log_level;
extern bool debug;

log_level get_min_log_level(void) {
  if (debug)
    return INFO;
  return WARN;
}

void log(log_level level, int status_code, const char *str, ...) {
  if (level < min_log_level)
    return;

  printf("\n/---------------------/\n");
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
  printf("STATUS CODE: %03d\n", status_code);

  va_list arg;
  va_start(arg, str);

  while (str) {
    printf("Argument: %s\n", str);
    str = va_arg(arg, const char *);
  }

  va_end(arg);
  puts("");
}
