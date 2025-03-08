#include "main.h"

int main(int argc, char **argv) {
  OS os;

  int rc = fork();

  // if fork() fails
  if (rc < 0)
    return -1;

  // if this is the main process, it should be nominated here
  if (rc > 0) {
    process p = {
        .name = "ChicOS", .pid = (i32)rc, .child = NULL, .status = READY};
    os.process_list[0] = p;
    log_process(os.process_list[0].pid, os.process_list);
  }
  // if a child process was created,
  if (rc == 0) {
    rc = fork();
    log(INFO, rc, "child process here\n", NULL);
  }

  return 0;
}
