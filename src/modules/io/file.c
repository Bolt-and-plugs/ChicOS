#include "file.h"
#include "../log/log.h"
#include "../memory/mem.h"

void read_header(file_buffer *fb) {
  file_header *header = alloc(sizeof(file_header));
  char s[256];

  fscanf(fb->fp, "%s", header->name);
  fscanf(fb->fp, "%d", &header->seg_flag);
  fscanf(fb->fp, "%d", &header->priority);
  fscanf(fb->fp, "%d", &header->seg_size);
  fscanf(fb->fp, "%s", s);

  while(strlen(s) <= 1) {
    strcat(header->semaphores,  s);
    fscanf(fb->fp, "%s", s);
  }
  printf("%s\n", header->semaphores);

  //fb->h->rw_count = counting_io_operations(fb);
}

i32 counting_io_operations(file_buffer *fb) {
  char aux[128];
  i32 count = 0;
  while (!feof(fb->fp)) {
    fscanf(fb->fp, "%s", aux);
    if (strcmp(aux, "write") == 0 || strcmp(aux, "read") == 0) {
      count++;
    }
  }

  char seeker[128];
  int value;
  while (!feof(fb->fp)) {
    fscanf(fb->fp, "%s %d", seeker, &value);
    printf("%s %d\n", seeker, value);
  }

  return count;
}

file_buffer *open_file(const char *address) {
  file_buffer *fb = alloc(sizeof(file_buffer));

  strcpy(fb->address, address);
  fb->fp = fopen(address, "r");

  if (!fb->fp) {
    c_error(DISK_OPEN_ERROR, "Could not open file");
    return NULL;
  }

  read_header(fb);

  return fb;
}

void close_file(file_buffer *fb) {
  if (fb->fp)
    fclose(fb->fp);
  dealloc(fb->h);
  dealloc(fb);
}
