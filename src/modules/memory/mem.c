#include "mem.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../utils/utils.h"

extern App app;

void init_mem(u32 mem_size) {
  if (mem_size >= 4 * MB) {
    c_error(MEM_ERROR, "Wrong value for initializing mem");
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

  if (sem_init(&app.mem->memory_s, 0, 0) != 0) {
    c_crit_error(SEMAPHORE_INIT_ERROR,"Memory semaphore failed to initialize")   
  }
}

void clear_mem() {
  if (!app.mem) {
    c_crit_error(MEM_ERROR, "There is no memory to be freed");
  }

  free(app.mem);

  char res[128];
  c_info(strcat(parse_int_to_string(MB, res), "B deallocated"));
}

void *alloc(u32 bytes) {
  if (bytes == 0)
    return NULL;

  sem_wait(&app.mem->memory_s);
  u32 num_pages = (bytes + PAGE_SIZE - 1) / PAGE_SIZE;

  if (num_pages > app.mem->free_page_num || num_pages > app.mem->len) {
    c_error(MEM_FULL, "Memory would overload when allocating");
    return NULL;
  }

  void *ptr = malloc(sizeof(alloc_header) + bytes);

  if (!ptr) {
    c_error(MEM_ALLOC_FAIL, "Failed to allocate memory");
    return NULL;
  }

  alloc_header *h_ptr = (alloc_header *)ptr;
  h_ptr->page_num = num_pages;
  h_ptr->size = bytes;

  app.mem->free_page_num -= num_pages;
  sem_post(&app.mem->memory_s);
  return (void *)((char *)ptr + sizeof(alloc_header));
}

alloc_header *get_header(void *ptr) {
  return (alloc_header *)((char *)ptr - sizeof(alloc_header));
}

void dealloc(void *mem) {
  if (!mem) {
    c_error(MEM_DEALLOC_FAIL,
            "Trying to deallocate a non allocated memory region");
    return;
  }
  alloc_header *h_ptr = get_header(mem);

  if (!(h_ptr->size > 0 && h_ptr->page_num > 0 &&
        h_ptr->page_num < app.mem->len))
    c_crit_error(MEM_DEALLOC_FAIL, "error");

  app.mem->free_page_num += h_ptr->page_num;

  free(h_ptr);
}

float retrieve_free_mem_percentage(void) {
  return (float) app.mem->free_page_num / (float)app.mem->len * 100.0f;
}

float retrieve_used_mem_percentage(void) {
  return 100.0f - retrieve_free_mem_percentage();
}
