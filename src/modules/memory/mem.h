#ifndef _MEMORY
#define _MEMORY

#include "../../defines.h"

#define MB 1048576
#define KB 1024

#define DEFAULT_MEMORY_SIZE MB
#define PAGE_SIZE KB

typedef struct __memory {
  u32 len;
  u32 free_page_num;
} memory;

typedef struct __alloc_header {
  u32 page_num;
  u32 size;
} alloc_header;

void init_mem(i32 mem_size);

void clear_mem(memory *mem);

void *alloc(u32 bytes);

alloc_header *get_header(void *ptr);

void dealloc(void *mem);

float retrieve_free_mem_percentage(void);

float retrieve_used_mem_percentage(void);

#endif
