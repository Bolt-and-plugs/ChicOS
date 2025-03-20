#ifndef _FILE
#define _FILE

#include "../../defines.h"

typedef struct file_buffer file_buffer;
typedef struct file_header file_header;

struct file_header {
  char *name;
  i32 priority;
  i32 seg_flag;
  char *seg_size;
  char *semaphores;
};

struct file_buffer {
  FILE *fp;
  char mode;
};

// create a synthetic file struct and another one for normal files (e.g we must
// load code from disk)?

file_buffer *open_file(const char *address, const char *mode);
void exec_file(file_buffer *file);
void close_file(void);

#endif
