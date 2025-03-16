#ifndef _ARENA
#define _ARENA

#include "../../defines.h"
#include "../log/log.h"
#include "../utils/utils.h"

#define ARENA_MAX_SIZE 4096

typedef struct Arena Arena;

struct Arena {
  unsigned char *buf;
  size_t buf_len;
  size_t prev_offset;
  size_t curr_offset;
};

Arena create_arena(i32 size);
void *alloc_arena(Arena a, size_t size);
uintptr_t align_forward(uintptr_t ptr, size_t align);
void free_arena(Arena a);

#endif
