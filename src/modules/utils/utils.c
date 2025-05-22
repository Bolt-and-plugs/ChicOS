#include "utils.h"
#include "assert.h"
#include "math.h"
#include "stdint.h"

void sleep_ms(u32 ms) { usleep(ms * 1000); }

void sleep_ms_with_time(u32 ms, u32 *ts) {
  for (u32 i = 0; i < ms; i++) {
    sleep_ms((u32)1);
    (*ts)--;
  }
}

int power(int a, int b) {
  int o = 1;
  for (int i = 0; i < b; i++) {
    o *= a;
  }

  return o;
}

bool is_power_of_two(uintptr_t x) { return (x & (x - 1)) == 0; }

bool valid_int(int v) { return (v > 0 && v < INT32_MAX) ? true : false; }

char *parse_int_to_string(int v, char *str) {
  int i = 0;
  int negative = v < 0 ? true : false;

  if (v < 0)
    v = -v;

  while (v > 0) {
    str[i++] = v % 10 + '0';
    v /= 10;
  }

  if (negative)
    str[i++] = '-';

  str[i] = '\0';

  for (int j = 0, k = i - 1; j < k; j++, k--) {
    char temp = str[j];
    str[j] = str[k];
    str[k] = temp;
  }

  return str;
}
