#include "mem.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../utils/utils.h"

extern App app;

void init_mem(u32 mem_size) {
  if (mem_size >= 4 * MB) {
    c_error(MEM_ERROR,
            "Given memory size %d is broken! Setting to default mem size: %",
            mem_size, DEFAULT_MEMORY_SIZE);
    mem_size = DEFAULT_MEMORY_SIZE;
  }

  memory *mem = malloc(sizeof(memory));
  void *memory_pool = malloc(mem_size);

  if (!mem || !memory_pool) {
    c_crit_error(MEM_ERROR, "Could not bootstrap memory! given mem_size: %d",
                 mem_size);
  }

  mem->pt.len = mem_size / PAGE_SIZE;
  mem->pt.pages = malloc(sizeof(page) * mem->pt.len);
  mem->pt.free_stack = malloc(sizeof(u32) * mem->pt.len);
  mem->pt.free_stack_top = 0;
  mem->pt.free_page_num = mem->pt.len;
  mem->pool = memory_pool;
  app.mem = mem;

  for (int i = 0; i < mem->pt.len; i++) {
    mem->pt.pages[i].id = i;
    mem->pt.pages[i].p = memory_pool + (i * PAGE_SIZE);
    mem->pt.pages[i].free = true;
    push_free_stack(i);
  }

  if (!mem->pt.pages) {
    c_crit_error(MEM_ERROR, "Could not allocate pages!");
  }

  if (sem_init(&app.mem->memory_s, 0, 1) != 0) {
    c_crit_error(SEMAPHORE_INIT_ERROR, "Memory semaphore failed to initialize")
  }

  c_info("%dB allocated", (int)mem_size);
}

void clear_mem() {
  if (!app.mem) {
    c_crit_error(MEM_ERROR, "There is no memory to be freed");
  }

  int mem = sizeof(app.mem->pool);

  free(app.mem->pt.pages);
  free(app.mem->pool);
  free(app.mem);

  c_info("%d sB deallocated", mem);
}

void *alloc(u32 bytes) {
  if (bytes == 0)
    return NULL;

  sem_wait(&app.mem->memory_s);
  u32 num_pages = (bytes + sizeof(alloc_header) + PAGE_SIZE - 1) / PAGE_SIZE;

  if (num_pages > app.mem->pt.free_page_num || num_pages > app.mem->pt.len) {
    c_error(MEM_FULL, "Memory would overload when allocating %d pages",
            num_pages);
    sem_post(&app.mem->memory_s);
    return NULL;
  }

  void *ptr = NULL;
  for (int i = 0; i < app.mem->pt.len; i++) {
    bool contiguos_region = true;
    if (i + num_pages >= app.mem->pt.len)
      break;
    for (int j = i; j < i + num_pages; j++) {
      if(!app.mem->pt.pages[j].free)
        contiguos_region = false;
    }

    if (!contiguos_region)
      continue;
    if (contiguos_region) {
      ptr = (void*)((char *)app.mem->pool + (i * PAGE_SIZE));
      app.mem->pt.free_page_num -= num_pages;
      for (int j = i; j < i + num_pages; j++) {
        app.mem->pt.pages[j].free = false;
      }
      alloc_header *h_ptr = ptr;
      h_ptr->id = i;
      h_ptr->page_num = num_pages;
      break;
    }
  }

  if (!ptr) {
    c_error(MEM_ALLOC_FAIL, "Failed to allocate memory");
    sem_post(&app.mem->memory_s);
    return NULL;
  }

  return (void *)(char *)ptr + sizeof(alloc_header);
}

void push_free_stack(u32 i) {
  if (app.mem->pt.free_stack_top < app.mem->pt.len) {
    app.mem->pt.free_stack[app.mem->pt.free_stack_top++] = i;
  }
}

void dealloc(void *mem) {
  sem_wait(&app.mem->memory_s);
  if (!mem) {
    c_error(MEM_DEALLOC_FAIL,
            "Trying to deallocate a non allocated memory region");
    sem_post(&app.mem->memory_s);
    return;
  }

  alloc_header *h_ptr = get_header(mem);

  for (u32 i = h_ptr->id; i < h_ptr->id + h_ptr->page_num; i++) {
    app.mem->pt.pages[i].free = true;
    push_free_stack(i);
  }
  app.mem->pt.free_page_num += h_ptr->page_num;

  sem_post(&app.mem->memory_s);
}

alloc_header *get_header(void *ptr) {
  return (alloc_header *)((char *)ptr - sizeof(alloc_header));
}

float retrieve_free_mem_percentage(void) {
  return (float)app.mem->pt.free_page_num / (float)app.mem->pt.len * 100.0f;
}

float retrieve_used_mem_percentage(void) {
  return 100.0f - retrieve_free_mem_percentage();
}
void print_page_table_status() {
  for (int i = 0; i < app.mem->pt.len; i++) {
    if (app.mem->pt.pages[i].p)
      c_info("%d %d", i, app.mem->pt.pages[i].id);
  }
}
