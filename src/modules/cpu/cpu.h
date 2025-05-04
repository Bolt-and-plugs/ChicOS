#ifndef _CPU
#define _CPU

#include "../../defines.h"
#include "../io/file.h"
#include "../process/process.h"
#include "../scheduler/scheduler.h"
#include "../semaphore/semaphore.h"

#define TIME_SLICE 10
#define MAX_TIME_MORE_PAGES 1000

typedef enum {
  process_interrupt = 1, // interrupção gerada pela execução da função processCreate e pelo término de operações de E/S
  process_create = 2,    // chamada para iniciar a criação de um processo no BCP *
  process_kill = 3,     // chamada para terminar a existência de um processo no BCP *
  disk_request = 4,      // chamada de operação de entrada/saída para disco
  disk_finish = 5,       // sinalização de final de E/S para disco
  mem_load_req = 6,       // chamada de operação de carregamento na memória
  mem_load_finish = 7,    // sinalização de final de carregamento
  fs_request = 8,        // chamada para operação no sistema de arquivos
  fs_finish = 9,         // sinalização de final de operação no sistema de arquivos
  semaphore_p = 10,      // tratamento de bloqueio de processo
  semaphore_v = 11,      // tratamento de desbloqueio de processo
  print_request = 14,    // chamada de operação de entrada/saída para impressão
  print_finish = 15,     // sinalização de final de E/S para impressão
} events;


typedef struct __cpu {
  u64 quantum_time;
  pthread_t cpu_t;
  sem_t cpu_s;
} cpu;

void *init_cpu(void *arg);

void cpu_loop();

void sys_call(events e, const char *str, ...);

void interrupt_control(events e, const char *str, ...);

void exec_program(process *sint_process);

#endif
