#ifndef _FILE
#define _FILE

#include "../../defines.h"
#include "fs.h"

typedef struct __synt_header {
  char name[128];
  i32 priority;
  i32 rw_count;
  i32 seg_flag;
  i32 seg_size;
  char semaphores[256];
} synt_header;

typedef struct __synt_buffer {
  FILE *fp;
  synt_header *h;
  char address[MAX_ADDRESS_SIZE];
  u64 PC;
} synt_buffer;

synt_buffer *open_file(const char *address);
void exec_file(synt_buffer *file);
void close_file(synt_buffer *fb);
bool valid_path(const char *path);
#endif
