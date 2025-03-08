#include "log.h"
#include "stdio.h"

void log(log_status level, int status_code, const char *str, ...) {
  printf("\n/---------------------/\n");
  printf("LEVEL: ");
  if (level == ERROR) {
    printf("ERROR\n");
  } else if (level == WARN) {
    printf("WARN\n");
  } else if (level == INFO) {
    printf("INFO\n");
  }

  printf("STATUS CODE: %03d\n", status_code);

#ifdef DEBUG
  printf("Debug mode is ON\n");
#else
  printf("Debug mode is OFF\n");
#endif

  va_list arg;
  va_start(arg, str);

  while (str) {
    printf("Argument: %s\n", str);
    str = va_arg(arg, const char *);
  }

  va_end(arg);
  puts("");
}
