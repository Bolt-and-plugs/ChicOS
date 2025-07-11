#ifndef _MEMORY
#define _MEMORY

#include "../../defines.h"
#include "../process/process.h"

#define MB 1048576
#define KB 1024

#define DEFAULT_MEMORY_SIZE MB
#define PAGE_SIZE KB

typedef struct __page {
  int id;
  void *p;
  bool free;
  bool used;
} page;

typedef struct __page_table {
  u32 len;
  u32 free_page_num;
  u32 *free_stack;
  u32 free_stack_top;
  page *pages;
} page_table;

typedef struct __memory {
  pthread_t render_t;
  sem_t memory_s;
  page_table pt;
  void *pool;
  u32 size;
} memory;

typedef struct __alloc_header {
  int id;
  u32 page_num;
} alloc_header;

typedef enum {
  CODE = 0x000,
  STACK = 0x001,
  HEAP = 0x002,
} segment;

void init_mem(u32 mem_size);

void clear_mem();

void *c_alloc(u32 bytes);

void *c_realloc(void *curr_region, u32 bytes);

alloc_header *get_header(void *ptr);

void c_dealloc(void *mem);

void push_free_stack(u32 i);

void print_page_table_status();

bool is_mem_free(void *ptr);

float retrieve_free_mem_percentage(void);

float retrieve_used_mem_percentage(void);

void memory_load_finish(process *p);

void memory_load_req(process *p, u32 bytes);

int second_chance();

#endif
