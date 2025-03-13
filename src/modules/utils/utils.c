#include "utils.h"
#include "assert.h"

bool is_power_of_two(uintptr_t x) {
	return (x & (x-1)) == 0;
}
