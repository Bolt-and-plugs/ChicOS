#include "main.h"

int main(int argc, char **argv) {
  process process_list[10];
  printf("current id %d\n", (int)getpid());
  process BCP = {
      .pid = (i32)getpid() + 1,
      .oid = 1,
      .parent_id = (i32)getpid(),
      .name = "ChicOS",
      .child = NULL,
      .status = READY
  };

  process_list[0] = BCP;
  log_process(BCP.pid, process_list);

  return 0;
}
