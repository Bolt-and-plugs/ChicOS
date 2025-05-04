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
  mem->size = mem_size;
  app.mem = mem;

  for (int i = 0; i < mem->pt.len; i++) {
    mem->pt.pages[i].id = i;
    mem->pt.pages[i].p = memory_pool + (i * PAGE_SIZE);
    mem->pt.pages[i].free = true;
    mem->pt.pages[i].used = false;
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

  int mem_size = app.mem->size;

  free(app.mem->pt.pages);
  free(app.mem->pt.free_stack);
  free(app.mem->pool);
  free(app.mem);

  c_info("%dB deallocated", mem_size);
}

void *c_alloc(u32 bytes) {
  c_info("Allocating %d bytes", bytes);
  if (bytes == 0)
    return NULL;

  u32 num_pages = (bytes + sizeof(alloc_header) + PAGE_SIZE - 1) / PAGE_SIZE;

  if (bytes >= app.mem->size) {
    c_error(MEM_ALLOC_FAIL,
            "greater chunk of memory than physical memory holds");
    return NULL;
  }

  c_info("Pages to be allocated: %d\n Free pages: %d", num_pages,
         app.mem->pt.free_page_num);
  if (num_pages > app.mem->pt.free_page_num || num_pages > app.mem->pt.len) {
    c_info("Not enough memory to allocate %d pages", num_pages);
    while (num_pages > app.mem->pt.free_page_num) {
      int not_used_page = second_chance();
      c_info("Not used page %d", not_used_page);
      if (not_used_page == -1) {
        c_error(MEM_FULL, "Memory full even after page replacement!");
        semaphoreV(&app.mem->memory_s);
        return NULL;
      }

      semaphoreP(&app.mem->memory_s);
      app.mem->pt.pages[not_used_page].free = true;
      app.mem->pt.free_page_num++;
      semaphoreV(&app.mem->memory_s);
      push_free_stack(not_used_page);
    }
  }

  semaphoreP(&app.mem->memory_s);
  void *ptr = NULL;
  for (int i = 0; i < app.mem->pt.len; i++) {
    bool contiguos_region = true;
    if (i + num_pages > app.mem->pt.len) {
      break;
    }
    for (int j = i; j < i + num_pages; j++) {
      if (!app.mem->pt.pages[j].free) {
        contiguos_region = false;
        break;
      }
    }

    if (!contiguos_region)
      continue;
    if (contiguos_region) {
      c_info("Found %d pages at %d", num_pages, i);
      ptr = (void *)((char *)app.mem->pool + (i * PAGE_SIZE));
      app.mem->pt.free_page_num -= num_pages;
      for (int j = i; j < i + num_pages; j++) {
        app.mem->pt.pages[j].free = false;
        app.mem->pt.pages[j].used = true;
      }
      alloc_header *h_ptr = ptr;
      h_ptr->id = i;
      h_ptr->page_num = num_pages;
      break;
    }
  }

  if (!ptr) {
    semaphoreV(&app.mem->memory_s);
    c_crit_error(MEM_ALLOC_FAIL,
                 "Failed to allocate %d bytes of memory with %d pages", bytes,
                 num_pages);
    return NULL;
  }

  semaphoreV(&app.mem->memory_s);
  c_debug(MEM_STATUS, "Allocated %d pages for %d bytes", num_pages, bytes);
  return (void *)(char *)ptr + sizeof(alloc_header);
}

void *c_realloc(void *curr_region, u32 bytes) {
  if (!curr_region) {
    c_error(MEM_REALLOC_FAIL, "NULL pointer passed to realloc");
    return NULL;
  }

  alloc_header *h_ptr = get_header(curr_region);
  if (!h_ptr) {
    c_error(MEM_REALLOC_FAIL, "No valid header found for memory: %p",
            curr_region);
    return NULL;
  }

  u32 old_size = h_ptr->page_num * PAGE_SIZE - sizeof(alloc_header);
  u32 total_size = old_size + bytes;

  void *buffer = c_alloc(total_size);
  if (!buffer) {
    c_error(MEM_REALLOC_FAIL, "Failed to realloc %d + %d bytes", old_size,
            bytes);
    return NULL;
  }

  memcpy(buffer, curr_region, old_size);
  c_dealloc(curr_region);

  return buffer;
}

void c_dealloc(void *mem) {
  semaphoreP(&app.mem->memory_s);
  if (!mem) {
    c_error(MEM_DEALLOC_FAIL,
            "Trying to deallocate a non allocated memory region");
    semaphoreV(&app.mem->memory_s);
    return;
  }

  alloc_header *h_ptr = get_header(mem);

  for (u32 i = h_ptr->id; i < h_ptr->id + h_ptr->page_num; i++) {
    app.mem->pt.pages[i].free = true;
    push_free_stack(i);
  }

  app.mem->pt.free_page_num += h_ptr->page_num;
  semaphoreV(&app.mem->memory_s);
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

void memory_load_req(void *dest, u32 bytes) {
  dest = c_alloc(bytes);
} // TODO save ptr into process

void memory_load_finish(void *dest) { c_dealloc(dest); }

bool is_mem_free(void *ptr) {
  if (!ptr)
    return true;

  sem_wait(&app.mem->memory_s);

  alloc_header *h_ptr = get_header(ptr);
  bool is_free = true;
  for (int i = 0; i < h_ptr->page_num; i++) {
    page *p = (page *)(char *)app.mem->pool + (i * PAGE_SIZE);
    if (!p->free) {
      is_free = false;
      break;
    }
  }
  sem_post(&app.mem->memory_s);
  return is_free;
}

int second_chance() {
  static int i = 0; // Variável estática marcando o ínicio da lista circular
  int curr = i;

  sem_wait(&app.mem->memory_s);
  do {
    c_info("Page %d", curr);
    page *p = &app.mem->pt.pages[curr];
    if (!(p->used)) {
      p->used = false; // Usa a  segunda chance da página
      sem_post(&app.mem->memory_s);
      return (i = (curr + 1) %
                  app.mem->pt.len); // Retorna o índice da página a ser
                                    // substituída e atualiza o ínicio da lista
    }
    p->used = false;
    curr = (curr + 1) %
           app.mem->pt.len; // Atualiza o valor da curr para a próxima página
  } while (curr != i - 1); // Continua até voltar ao início da lista
  i = (curr + 1) % app.mem->pt.len;

  sem_post(&app.mem->memory_s);
  return -1;
}
