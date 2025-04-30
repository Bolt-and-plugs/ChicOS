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
  pthread_t render_t;
  sem_t memory_s;
} memory;

typedef struct __alloc_header {
  u32 page_num;
  u32 size;
} alloc_header;

typedef enum {
  CODE = 0x000,
  STACK = 0x001,
  HEAP = 0x002,
} segment;

typedef struct __page_table {
  u32 base;
  u32 bounds;
  u8 magic_number;
} page_table;

void init_mem(u32 mem_size);

void clear_mem();

void *alloc(u32 bytes);

alloc_header *get_header(void *ptr);

void dealloc(void *mem);

float retrieve_free_mem_percentage(void);

float retrieve_used_mem_percentage(void);

#endif
