#include "file.h"
#include "../log/log.h"
#include "../process/process.h"

file_buffer *open_file(const char *address, const char *mode) {
  if (strlen(mode) > 2)
    c_error(DISK_ERROR, "Mode size bigger than allowed!");

  file_buffer *fb = malloc(sizeof(file_buffer));
  strcpy(fb->address, address);
  strcpy(fb->mode, mode);

  if (!fb)
    c_error(DISK_FILE_READ_ERROR, "Could not create file buffer!!");
  return fb;
}

file_header *get_header() {}

// openning the sinthetic file to simulate the execution
void exec_file(file_buffer *file) {
  file->fp = fopen(file->address, &file->mode[0]);
  if (file->fp == NULL)
    c_error(DISK_FILE_READ_ERROR, "Error on disk file read");

  file_header header;
  header.name = malloc(sizeof(char) * MAX_SIZE_PROC_NAME);
  header.semaphores = malloc(sizeof(char) * MAX_SIZE_SEMAPHORES);

  void *temp_seg_size = malloc(80);

  fgets(temp_seg_size, sizeof(header.name), file->fp);
  fgets(temp_seg_size, sizeof(header.seg_flag), file->fp);
  fgets(temp_seg_size, sizeof(header.priority), file->fp);
  fgets(temp_seg_size, sizeof(header.seg_size), file->fp);
  printf("%d", (int)temp_seg_size);
  return;
  fgets(header.semaphores, sizeof(header.semaphores), file->fp);

void close_file(void) {}
