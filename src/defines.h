#ifndef _DEFINES
#define _DEFINES

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
#include <sys/wait.h>
#endif
#include <time.h>
#include <unistd.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef enum {
  process_interrupt = 1,
  process_create = 2,
  process_kill = 3,
  disk_request = 4,
  disk_finish = 5,
  mem_load_req = 6,
  mem_load_finish = 7,
  semaphore_p = 8,
  semaphore_v = 9,
  print_request = 10,
  print_finish = 11,
  process_exec = 12,
} events;
#endif
