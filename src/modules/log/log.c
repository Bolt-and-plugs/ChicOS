#include "log.h"
#include "../../chicos.h"
#include "../render/render.h"
#include "stdio.h"
#include "../utils/utils.h"

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
      printf("\n[ERROR ");
      break;
    case WARN:
      printf("\n[WARN ");
      break;
    case INFO:
      printf("\n[INFO ");
      break;
    case DEBUG:
      printf("\n[DEBUG ");
      break;
    case CRIT_ERROR:
      printf("\n[CRIT_ERROR ");
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
      printf("| %d - SCHEDULER ", status);
      break;
    case 1:
      printf("| %d - Process Handling Context ", status);
      break;
    case 2:
      printf("| %d - CPU Related Context ", status);
      break;
    case 3:
      printf("| %d - Memory Status Context ", status);
      break;
    case 5:
      printf("| %d - User Status Context ", status);
      break;
    default:
      printf("| %d - Unknown Status ", status);
    }
  }

  char buffer[4096];

  va_list arg_list;
  va_start(arg_list, str);
  vsnprintf(buffer, 255, str, arg_list);
  va_end(arg_list);
  time_t clk = time(NULL);
  if (!app.rdr.active) {
    printf(" %s] \n %s\n", sanitize_str(ctime(&clk)), buffer);
  }

  if (app.rdr.active && app.debug)
    render_log(buffer);
}
