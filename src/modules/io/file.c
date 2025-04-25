#include "file.h"
#include "../log/log.h"
#include "../memory/mem.h"
#include "../process/process.h"

file_header *read_header(file_buffer *fb) {
  open_file(fb->address);
  file_header *header = malloc(sizeof(file_header));
  int i = 1;
  fscanf(fb->fp, "%s", header->name);
  fscanf(fb->fp, "%d", header->seg_flag);
  fscanf(fb->fp, "%d", header->priority);
  fscanf(fb->fp, "%d", header->seg_size);
  fscanf(fb->fp, "%c", header->semaphores[0]);
  while(header->semaphores[i] != '\n'){
    fscanf(fb->fp, "%c", header->semaphores[i]);
    i++;
  }
  fb->h->rw_count = counting_io_operations(fb);

  return header;
}

i32 counting_io_operations(file_buffer *fb) {
  char aux[16];
  i32 count=0;
  while(!feof(fb->fp)){
    fscanf(fb->fp, "%s", aux);
    if(strcmp(aux, "write") == 0 || strcmp(aux, "read") == 0) {
      count++;
    }
  }

  char seeker[128];
  fseek(fb->fp, 0, SEEK_SET);
  while(seeker != '\n') {
    fscanf(fb->fp, "%s", seeker);
  }
  
  return count;
}

file_buffer *open_file(const char *address) {
  file_buffer *fb = alloc(sizeof(file_buffer));

  strcpy(fb->address, address);
  fb->fp = fopen(address, "r");

  if (!fb->fp) {
    c_error(DISK_OPEN_ERROR, "Could not open file");
    return;
  }

  file_header *header = read_header(fb);
  header->rw_count = counting_io_operations(fb);

  return fb;
}

void close_file(file_buffer *fb) {
  if (fb->fp)
    fclose(fb->fp);
  dealloc(fb);
}

// openning the sinthetic file to simulate the execution
void open_sinthetic_file(file_buffer *file) {
  file_buffer *fb = malloc(sizeof(file_buffer));
  fb->h = read_header(fb); 
}
