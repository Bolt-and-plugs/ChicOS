#include "mem.h"
#include "../log/log.h"
#include "../utils/utils.h"
#include "chicos.h"

extern App app;

void init_mem() {
  memory *mem = malloc(MB);
  mem->len = MEMORY_SIZE / PAGE_SIZE;

  if (!mem) {
    c_log(CRIT_ERROR, MEM_ERROR, "could not bootstrap memory!", NULL);
    exit(0);
  }

  char res[128];
  c_log(INFO, MEM_STATUS, strcat(parse_int_to_string(MB, res), "B allocated"),
        NULL);

  app.mem = mem;
}

void clean_mem(memory *mem) {
  if (!mem) {
    c_log(CRIT_ERROR, MEM_ERROR, "There is no memory to free", NULL);
    exit(0);
  }

  free(mem);

  char res[128];
  c_log(INFO, MEM_STATUS, strcat(parse_int_to_string(MB, res), "B deallocated"),
        NULL);
}
