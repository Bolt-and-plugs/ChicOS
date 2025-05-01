#include "log.h"
#include "../../chicos.h"
#include "../render/render.h"
#include "stdio.h"

extern App app;

log_level get_min_log_level(void) {
  if (app.debug)
    return INFO;
  return WARN;
}

void c_log(log_level level, status_code status, const char *str, ...) {
  if (level < app.min_log_level)
    return;

  if (!app.rdr.active) {
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
  }

  char buffer[4096];

  va_list arg_list;
  va_start(arg_list, str);
  vsnprintf (buffer, 255, str, arg_list);
  va_end(arg_list);
  time_t clk = time(NULL);
  if (!app.rdr.active)
    printf("[%s] - %s", ctime(&clk) ,buffer);

  if (app.rdr.active)
    render_log(buffer);

  puts("");
}
