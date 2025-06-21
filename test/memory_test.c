#include "../src/modules/memory/mem.h"
#include <assert.h>

int memory_test(void) {
  int *result1 = c_alloc(sizeof(int));
  assert(result1); 

  void *result2 = c_alloc(4096);
  assert(result1); 

  return 0;
}
