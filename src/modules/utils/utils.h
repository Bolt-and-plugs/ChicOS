#ifndef _UTILS
#define _UTILS

#include "../../defines.h"

bool is_power_of_two(uintptr_t x);

int parse_string_to_int(char *str);

char *parse_int_to_string(int v, char *str);

bool valid_int(int v);

#endif
