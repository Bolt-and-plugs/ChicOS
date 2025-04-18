#include "mem.h"
#include "../log/log.h"
#include "../utils/utils.h"
#include "../../chicos.h"

extern App app;

void init_mem(i32 mem_size) {
  if (mem_size >= 4 * MB) {
    c_error(MEM_ERROR, "Testing");
  }
  memory *mem = malloc(mem_size);

  if (!mem) {
    c_crit_error(MEM_ERROR, "could not bootstrap memory!");
    exit(0);
  }

  mem->len = MEMORY_SIZE / PAGE_SIZE;

  char res[128];
  c_info(strcat(parse_int_to_string(mem_size, res), "B allocated"));

  app.mem = mem;
}

void clean_mem(memory *mem) {
  if (!mem) {
    c_crit_error(MEM_ERROR, "There is no memory to free");
    exit(0);
  }

  free(mem);

  char res[128];
  c_info(strcat(parse_int_to_string(MB, res), "B deallocated"));
}
