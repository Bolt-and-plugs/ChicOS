#include "utils.h"
#include "assert.h"
#include "stdint.h"

void sleep_ms(u32 ms) { usleep(ms * 1000); }

void sleep_ms_with_time(u32 ms, u32 *ts) {
  for (u32 i = 0; i < ms; i++) {
    sleep_ms((u32)1);
    (*ts)--;
  }
}

bool valid_int(int v) { return (v > 0 && v < INT32_MAX) ? true : false; }

u32 abs_diff(u32 a, u32 b) { return a > b ? a - b : b - a; }

char *sanitize_str(char *str) {
  if (!str) {
    return NULL;
  }
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == '\n' || str[i] == '-') {
      str[i] = ' ';
    }
  }

  return str;
}
