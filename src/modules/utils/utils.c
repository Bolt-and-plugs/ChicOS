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

bool is_power_of_two(uintptr_t x) { return (x & (x - 1)) == 0; }

bool valid_int(int v) { return (v > 0 && v < INT32_MAX) ? true : false; }
