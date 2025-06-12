#ifndef _FS
#define _FS

#include "../../defines.h"

#define MAX_ADDRESS_SIZE 4096

typedef struct __inode {
  u32 inode_num;

} inode;

typedef struct __file_desc {
  inode i;
} file_desc;

int c_write(file_desc *fd);
int c_read(file_desc *fd);
file_desc *c_open(const char *path);

#endif
