#include "main.h"

int main(int argc, char **argv) {
  OS os;
  printf("current id %d\n", (int)getpid());
  os.process_list[0] = (process){.name = "ChicOS",
                                 .pid = (i32)getpid() + 1,
                                 .oid = 1,
                                 .parent_id = (i32)getpid(),
                                 .child = NULL,
                                 .status = READY};

  log_process(os.process_list[0].pid, os.process_list);
  return 0;
}
