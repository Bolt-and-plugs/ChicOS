#include "mem.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../utils/utils.h"

extern App app;

void init_mem(i32 mem_size) {
  if (mem_size >= 4 * MB) {
    c_error(MEM_ERROR, "Testing");
  }
  memory *mem = malloc(mem_size);

  if (!mem) {
    c_crit_error(MEM_ERROR, "could not bootstrap memory!");
  }

  mem->len = mem_size / PAGE_SIZE;
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
  u32 num_pages = bytes / PAGE_SIZE;
  app.mem->free_page_num -= num_pages;

  if (app.mem->free_page_num - num_pages <= 0 || num_pages >= app.mem->len) {
    c_error(MEM_FULL, "Memory would overload when allocating this object");
    return NULL;
  }

  return malloc(sizeof(bytes));
}

void dealloc(void *mem) {
  if (!mem) {
    c_crit_error(MEM_ERROR,
                 "Trying to deallocate a non allocated memory region");
  }

  u32 num_pages = sizeof(mem) / PAGE_SIZE;
  app.mem->free_page_num += num_pages;
}
