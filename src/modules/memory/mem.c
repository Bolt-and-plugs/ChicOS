#include "mem.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../utils/utils.h"

extern App app;

void init_mem(i32 mem_size) {
  if (mem_size >= 4 * MB) {
    c_error(MEM_ERROR, "Testing");
    mem_size = DEFAULT_MEMORY_SIZE;
  }
  memory *mem = malloc(sizeof(memory));

  if (!mem) {
    c_crit_error(MEM_ERROR, "could not bootstrap memory!");
  }

  mem->len = mem_size / PAGE_SIZE;
  mem->free_page_num = mem->len;

  char res[128];
  c_info(strcat(parse_int_to_string(mem_size, res), "B allocated"));

  app.mem = mem;
}

void clear_mem(memory *mem) {
  if (!mem) {
    c_crit_error(MEM_ERROR, "There is no memory to free");
  }

  free(mem);

  char res[128];
  c_info(strcat(parse_int_to_string(MB, res), "B deallocated"));
}

void *alloc(u32 bytes) {
  if (bytes == 0)
    return NULL;

  u32 num_pages = (bytes + PAGE_SIZE - 1) / PAGE_SIZE;

  if (num_pages > app.mem->free_page_num || num_pages > app.mem->len) {
    c_error(MEM_FULL, "Memory would overload when allocating");
    return NULL;
  }

  void *ptr = malloc(bytes);
  alloc_header h_ptr = {}; // todo

  if (!ptr) {
    c_error(MEM_ALLOC_FAIL, "Failed to allocate memory");
    return NULL;
  }

  app.mem->free_page_num -= num_pages;
  return ptr;
}

void dealloc(void *mem) {
  if (!mem) {
    c_error(MEM_DEALLOC_FAIL,
            "Trying to deallocate a non allocated memory region");
    return;
  }

  u32 num_pages = (sizeof(mem) + PAGE_SIZE - 1) / PAGE_SIZE;
  app.mem->free_page_num += num_pages;
}

float retrieve_free_mem_percentage(void) {
  return (float)app.mem->free_page_num / (float)app.mem->len * 100.0f;
}

float retrieve_used_mem_percentage(void) {
  return 100.0f - retrieve_free_mem_percentage();
}
