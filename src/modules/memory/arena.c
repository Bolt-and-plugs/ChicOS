#include "arena.h"
#include "../log/log.h"
#include "../utils/utils.h"
#include "assert.h"
#include <sys/mman.h>

#define HEAP_SIZE 640000
char heap[HEAP_SIZE] = {0};
size_t heap_size = 0;

Arena *create_arena(i32 size) {
  if (size < 0 || size > ARENA_MAX_SIZE) {
    size = ARENA_MAX_SIZE;
    c_log(WARN, 0, "Fall back arena size to ARENA_MAX_SIZE", NULL);
  }

  assert(heap_size + size <= HEAP_SIZE);

  void *ptr = heap + heap_size;
  heap_size += size;

  char s[128];
  c_log(DEBUG, MEM_STATUS, "Allocated ", parse_int_to_string(size, s), NULL);

  return ptr;
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

void *alloc_arena(Arena *a, size_t s) {
  if ((i32)s >= (i32)a->buf_len) {
    char res[128];
    c_log(ERROR, MEM_STATUS, "Size bigger than arena max!",
          parse_int_to_string(s, res), parse_int_to_string(a->buf_len, res),
          NULL);
    return NULL;
  }
  if (s + a->curr_offset > a->buf_len) {
    c_log(ERROR, MEM_STATUS, "Allocating would break arena max size!", NULL);
    return NULL;
  }

  align_forward(*a->buf, s);

  return a->buf + s;
}

void free_arena(Arena a) {
  if (!a.buf) {
    free(a.buf);
  }
}
