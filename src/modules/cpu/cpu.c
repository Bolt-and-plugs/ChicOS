#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../log/log.h"
#include "../file/file.h"
#include "../process/process.h"
#define TIME_SLICE 10 // time slice duration in ms

void exec_program(file_buffer *sint, process *sint_process){
    char *command, aux[16];
    int time
    if(sint->fp == NULL){
        printf("File not open properly!");
        return;
    }
    for(int i = 0; i < 6; i++)
        fgets(aux, sizeof(aux), sint->fp);
    while(!feof(sint->fp)){
        fgets(aux, sizeof(aux), sint->fp);
        command = strtok(aux, " ")
        time = atoi(strok(NULL, " "))
        if(strcmp(command, "exec") == 0){
            printf("Executing program for %dms...", time)
            sleep(time / 1000);
        }
        else if(strcmp(command, "write") == 0){
            printf("Writing on dik for %dms...", time)
            sleep(time / 1000);
        }
        else if(strcmp(command, "read") == 0){
            printf("Reading on dik for %dms...", time)
            sleep(time / 1000);
        }
        else if(strcmp(command, "P") == 0){

        }
        else if(strcmp(command, "V") == 0){

        }
        else if(strcmp(command, "print") == 0){

        }
        else {
            c_error(DISK_FILE_READ_ERROR, "Found invalid command!")
        }
    }
}