#include "file.h"

file_buffer *open_file(const char *address, char *mode)
{
  file_buffer *fb = malloc(sizeof(file_buffer));
  strcpy(fb->adress, address);
  fb->mode = *mode;

  return fb;
}

// openning the sinthetic file to simulate the execution
void exec_file(file_buffer *file)
{
  file->fp = fopen(file->adress, file->mode);
  if(file->fp == NULL)
    c_log(ERROR, DISK_FILE_READ_ERROR, "Error on disk file read", NULL);

  file_header header;
  header.name = malloc(sizeof(char)*MAX_SIZE_PROC_NAME);
  header.semaphores = malloc(sizeof(char)*MAX_SIZE_SEMAPHORES);
  
  fgets(header.name, sizeof(header.name), file->fp);
  fgets(header.seg_flag, sizeof(header.seg_flag), file->fp);
  fgets(header.priority, sizeof(header.priority), file->fp);
  fgets(header.seg_size, sizeof(header.seg_size), file->fp);
  fgets(header.semaphores, sizeof(header.semaphores), file->fp);

  // identifing the command and simulating him
  while(!feof(file))
  {
    char process[16];
    fgets(&process[0], sizeof(char)*16, file->fp);
    
    char *identifier, *aux;
    identifier = strtok(process, " ");
    aux = strtok(NULL, " ");

    if(identifier != NULL)
    {
      if(aux != NULL)
      {
        if(strcmp(identifier, "exec"))
        {
          int time = (atoi(aux)/200)+1;
          printf("Executing IO request...\n");
          sleep(time);
          printf("IO request finished\n");
        }else
        {
          if(strcmp(identifier, "read"))
          {
            int k = atoi(aux);
            printf("Reading from disk on trail %d...\n", k);
            sleep(3);
            printf("Reading finished\n");
          }else
          {
            if(strcmp(identifier, "write"))
            {
              int k = atoi(aux);
              printf("Writing in disk on trail %d...\n", k);
              sleep(3);
              printf("Writing finished\n");
            }else
            {
              if(strcmp(identifier, "print"))
              {
                int time = (atoi(aux)/200)+1;
                printf("Printing...\n");
                sleep(time);
                printf("Writing finished\n");
              }else
              {
                if(process[0] == 'P')
                {
                  printf("Acessing critic lane %c\n", process[2]);
                }else if(process[0] == 'V')
                {
                  printf("Free critic lane %c\n", process[2]);
                }else
                {
                  c_log(ERROR, DISK_FILE_READ_ERROR, "The command in file is not valid", NULL);
                }
              }              
            }
          }
        }
      }
    }
  }
}
void close_file(void)
{

}