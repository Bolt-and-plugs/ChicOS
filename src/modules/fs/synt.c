#include "synt.h"
#include "../log/log.h"
#include "../memory/mem.h"
#include "../semaphore/semaphore.h"

bool valid_path(const char *path) {
  if (!path || strlen(path) > MAX_ADDRESS_SIZE || access(path, F_OK) != 0) {
    return false;
  }

  return true;
}

void read_header(synt_buffer *fb) {
  synt_header *header = c_alloc(sizeof(synt_header));
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
    if (c != ' '){
      header->semaphores[i++] = c;
      init_semaphore(c, 1);
    }
    c = fgetc(fb->fp);
  }

  header->semaphores[i] = '\0';

  fb->h = header;
  fb->h->rw_count = 0;
}

synt_buffer *open_file(const char *address) {
  synt_buffer *sb = c_alloc(sizeof(synt_buffer));

  if (!address || strlen(address) >= MAX_ADDRESS_SIZE) {
    c_error(INVALID_INPUT, "address is bigger than max address or is missing");
    return NULL;
  }

  strcpy(sb->address, address);
  sb->fp = fopen(address, "r");

  if (!sb->fp) {
    c_error(DISK_OPEN_ERROR, "Could not open file");
    return NULL;
  }

  read_header(sb);

  return sb;
}

void close_file(synt_buffer *fb) {
  if (fb->fp)
    fclose(fb->fp);
  c_dealloc(fb->h);
  c_dealloc(fb);
}