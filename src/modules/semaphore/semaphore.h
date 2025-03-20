#ifndef _SEMAPHORE
#define _SEMAPHORE

typedef enum {
  process_interrupt = 1, // interrupção gerada pela execução da função processCreate e pelo término de operações de E/S
  process_create = 2,    // chamada para iniciar a criação de um processo no BCP *
  process_finish = 3,     // chamada para terminar a existência de um processo no BCP *
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

void interrupControl(void);
void sysCall(void);

#endif
