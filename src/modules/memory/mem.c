#include "mem.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../utils/utils.h"

extern App app;

void init_mem(i32 mem_size) {
  if (mem_size >= 4 * MB) {
    c_error(MEM_ERROR,
            "Given memory size is broken! Setting to default mem size");
    mem_size = DEFAULT_MEMORY_SIZE;
  }
  memory *mem = malloc(mem_size);

  if (!mem) {
    c_crit_error(MEM_ERROR, "could not bootstrap memory!");
  }

  mem->len = mem_size / PAGE_SIZE;

  char res[128];
  c_info(strcat(parse_int_to_string(MB, res), "B allocated"));

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
