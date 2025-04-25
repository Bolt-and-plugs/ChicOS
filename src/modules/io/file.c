#include "file.h"
#include "../log/log.h"
#include "../memory/mem.h"
#include "../process/process.h"

file_header *read_header(FILE *fp) {
  file_header *header = alloc(sizeof(file_header));
  int i = 0;
  fscanf(fp, "%s", header->name);
  fscanf(fp, "%d", &header->seg_flag);
  fscanf(fp, "%d", &header->priority);
  fscanf(fp, "%d", &header->seg_size);
  fscanf(fp, "%c", header->semaphores);
  i++;
  while (header->semaphores[i] != '\n') {
    char c;
    fscanf(fp, "%c", &c);
    if (c != ' ') {
      header->semaphores[i] = c;
    }
    printf("%c", header->semaphores[i]);
  }

  return header;
}

file_buffer *open_file(const char *address) {
  file_buffer *fb = alloc(sizeof(file_buffer));

  strcpy(fb->address, address);
  fb->fp = fopen(address, "r");

  if (!fb->fp) {
    c_error(DISK_OPEN_ERROR, "Could not open file");
    return NULL;
  }

  read_header(fb->fp);

  return fb;
}

void close_file(file_buffer *fb) {
  if (fb->fp)
    fclose(fb->fp);
  dealloc(fb);
}
