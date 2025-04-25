#ifndef _FILE
#define _FILE

#include "../../defines.h"

#define MAX_ADDRESS_SIZE 4096

typedef struct file_buffer file_buffer;
typedef struct file_header file_header;

struct file_header {
  char name[128];
  i32 priority;
  i32 rw_count;
  i32 seg_flag;
  i32 seg_size;
  char semaphores[256];
};

struct file_buffer {
  FILE *fp;
  file_header *h;
  char address[MAX_ADDRESS_SIZE];
  u64 PC;
};

file_buffer *open_file(const char *address);
void exec_file(file_buffer *file);
void close_file(file_buffer *fb);
i32 counting_io_operations(file_buffer *fb);

#endif
