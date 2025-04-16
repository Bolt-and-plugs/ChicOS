#ifndef _FILE
#define _FILE

#include "../../defines.h"

#define MAX_ADDRESS_SIZE 4096

typedef struct file_buffer file_buffer;
typedef struct file_header file_header;

struct file_header {
  char *name;
  i32 priority;
  i32 seg_flag;
  i32 seg_size;
  char *semaphores;
};

struct file_buffer {
  FILE *fp;
  char address[MAX_ADDRESS_SIZE];
  char mode[2];
};

// create a synthetic file struct and another one for normal files (e.g we must
// load code from disk)?

file_buffer *open_file(const char *address, const char *mode);
void exec_file(file_buffer *file);
void close_file(void);

#endif
