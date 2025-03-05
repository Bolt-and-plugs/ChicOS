#include "process.h"


int p_create(i32 pid) {
  process p;
  p.pid = fork();
  p.parent_id = pid;
  // TODO add to list
  return p.pid;
}


void log_process(i32 pid) {
  

}


