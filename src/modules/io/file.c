#include "file.h"
#include "../log/log.h"
#include "../process/process.h"

file_buffer *open_file(const char *address, const char *mode) {
  if (strlen(mode) > 2)
    c_log(ERROR, DISK_ERROR, "Mode size bigger than allowed!", NULL);

  file_buffer *fb = malloc(sizeof(file_buffer));
  strcpy(fb->address, address);
  strcpy(fb->mode, mode);

  if (!fb)
    c_log(ERROR, DISK_FILE_READ_ERROR, "Could not create file buffer!!", NULL);
  return fb;
}

file_header *get_header() {}

// openning the sinthetic file to simulate the execution
void exec_file(file_buffer *file) {
  file->fp = fopen(file->address, &file->mode[0]);
  if (file->fp == NULL)
    c_log(ERROR, DISK_FILE_READ_ERROR, "Error on disk file read", NULL);

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

  // identifing the command and simulating him
  while (!feof(file->fp)) {
    char process[16];
    fgets(&process[0], sizeof(char) * 16, file->fp);

    char *identifier, *aux;
    identifier = strtok(process, " ");
    aux = strtok(NULL, " ");

    if (identifier != NULL) {
      if (aux != NULL) {
        if (strcmp(identifier, "exec") == 0) {
          int time = (atoi(aux) / 200) + 1;
          printf("Executing IO request...\n");
          sleep(time);
          printf("IO request finished\n");
        } else {
          if (strcmp(identifier, "read") == 0) {
            int k = atoi(aux);
            printf("Reading from disk on trail %d...\n", k);
            sleep(3);
            printf("Reading finished\n");
          } else {
            if (strcmp(identifier, "write") == 0) {
              int k = atoi(aux);
              printf("Writing in disk on trail %d...\n", k);
              sleep(3);
              printf("Writing finished\n");
            } else {
              if (strcmp(identifier, "print") == 0) {
                int time = (atoi(aux) / 200) + 1;
                printf("Printing...\n");
                sleep(time);
                printf("Writing finished\n");
              } else {
                if (process[0] == 'P') {
                  printf("Acessing critic lane %c\n", process[2]);
                } else if (process[0] == 'V') {
                  printf("Free critic lane %c\n", process[2]);
                } else {
                  c_log(ERROR, DISK_FILE_READ_ERROR,
                        "The command in file is not valid", NULL);
                }
              }
            }
          }
        }
      }
    }
  }
}

void close_file(void) {}
