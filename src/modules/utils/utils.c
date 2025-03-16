#include "utils.h"
#include "assert.h"

bool is_power_of_two(uintptr_t x) {
	return (x & (x-1)) == 0;
}

int parse_string_to_int(char *str) {
  int size = 0;
  int length = strlen(str);
  for (int i = 0, j = length - 1; i < length; i++, j--) {
    size = size * 10 + (str[i] - '0');
  }
  return size;
}
