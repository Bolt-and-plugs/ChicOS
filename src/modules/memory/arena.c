#include "arena.h"
#include "../log/log.h"
#include "../utils/utils.h"
#include "assert.h"

Arena create_arena(i32 size) {
  // TODO
  Arena a = {.buf = 0, .buf_len = size, .curr_offset = 0, .prev_offset = 0};
  return a;
}

uintptr_t align_forward(uintptr_t ptr, size_t align) {
  uintptr_t p, a, modulo;

  assert(is_power_of_two(align));

  p = ptr;
  a = (uintptr_t)align;
  // Same as (p % a) but faster as 'a' is a power of two
  modulo = p & (a - 1);

  if (modulo != 0) {
    // If 'p' address is not aligned, push the address to the
    // next value which is aligned
    p += a - modulo;
  }
  return p;
}

void *alloc_arena(Arena a, size_t s) {
  if (s >= ARENA_MAX_SIZE) {
    c_log(ERROR, MEM_STATUS, "Size bigger than arena max!", NULL);
    return NULL;
  }
  if (s + a.curr_offset > ARENA_MAX_SIZE) {
    c_log(ERROR, MEM_STATUS, "Allocating would break arena max size!", NULL);
    return NULL;
  }
  // TODO
  align_forward(*a.buf, s);
  // handle arena alloc

  return malloc(100);
}

void free_arena(Arena a) {
  if (!a.buf) {
    free(a.buf);
  }
}
