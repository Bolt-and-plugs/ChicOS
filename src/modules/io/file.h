#ifndef _FILE
#define _FILE

#include "../../defines.h"

typedef struct file_buffer file_buffer;

struct file_buffer {
  FILE *fp;
  char mode;
  char name[];
};

file_buffer open_file(const char *address, const char *mode);
void exec_file(file_buffer *file);
void close_file(void);

#endif
