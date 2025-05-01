#include "file.h"
#include "../log/log.h"
#include "../memory/mem.h"

void read_header(file_buffer *fb) {
  file_header *header = c_alloc(sizeof(file_header));
  char c;
  int i = 0;
  char aux[256];

  fscanf(fb->fp, "%s", aux);
  if (strlen(aux) >= 127) {
    strncpy(header->name, aux, 127);
    header->name[127] = '\0';
  } else
    strcpy(header->name, aux);

  fscanf(fb->fp, "%d", &header->seg_flag);
  fscanf(fb->fp, "%d", &header->priority);
  fscanf(fb->fp, "%d", &header->seg_size);

  // has to be done twice in order to skip \n
  c = fgetc(fb->fp);
  c = fgetc(fb->fp);

  while (c != '\n') {
    if (c != ' ')
      header->semaphores[i++] = c;
    c = fgetc(fb->fp);
  }

  header->semaphores[i] = '\0';

  fb->h = header;
  fb->h->rw_count = counting_io_operations(fb);
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
  }

  return count;
}

file_buffer *open_file(const char *address) {
  file_buffer *fb = c_alloc(sizeof(file_buffer));

  if (!address || strlen(address) >= MAX_ADDRESS_SIZE) {
    c_error(INVALID_INPUT, "address is bigger than max address or is missing");
    return NULL;
  }

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
  c_dealloc(fb->h);
  c_dealloc(fb);
}
