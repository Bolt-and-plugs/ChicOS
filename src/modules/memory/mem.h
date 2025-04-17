#ifndef _MEMORY
#define _MEMORY

#include "../../defines.h"

#define MB 1048576
#define KB 1024

#define MEMORY_SIZE MB
#define PAGE_SIZE KB
#define PAGE_NUM 16

typedef struct __page {
  void *content;
  char present;
} page;

typedef struct __memory {
  page *arr;
  u32 len;
  u32 free_page_num;
} memory;

//
void init_mem(i32 mem_size);

void clean_mem(memory *mem);

//
page *alloc_pages();

//
void free_page(page *p);

#endif
