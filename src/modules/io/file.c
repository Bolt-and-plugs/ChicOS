#include "file.h"
#include "../log/log.h"
#include "../memory/mem.h"
#include "../process/process.h"

file_buffer *open_file(const char *address) {
  file_buffer *fb = alloc(sizeof(file_buffer));

  strcpy(fb->address, address);
  fb->fp = fopen(address, "r");

  if (!fb->fp) {
    c_error(DISK_OPEN_ERROR, "Could not open file");
  }

  return fb;
}

void close_file(file_buffer *fb) {
  if (fb->fp)
    fclose(fb->fp);
  dealloc(fb);
}

// openning the sinthetic file to simulate the execution
void open_sinthetic_file(file_buffer *file) {}
