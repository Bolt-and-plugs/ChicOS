#include "utils.h"
#include "assert.h"
#include "math.h"
#include "stdint.h"

void sleep_ms(int ms) { usleep(ms * 1000); }

bool is_power_of_two(uintptr_t x) { return (x & (x - 1)) == 0; }

int parse_string_to_int(char *str) {
  int size = 0;
  int length = strlen(str);
  int val;
  for (int i = 0, j = length - 1; i < length; i++, j--) {
    val = (str[i] - '0');
    if (val > 0 && val < INT32_MAX / pow(10, i))
      size = size * 10 + val;
  }
  if (size < 0 || val >= INT32_MAX)
    return 0;
  return size;
}

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
