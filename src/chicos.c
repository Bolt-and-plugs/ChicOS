#include "chicos.h"

log_level min_log_level;
bool debug;

bool set_envvar(const char *mode) {
  if (strcmp(mode, "Debug") == 0 || strcmp(mode, "DEBUG") == 0) {
    debug = true;
    return true;
  }

  debug = false;
  return false;
}

int main(int argc, char **argv) {
  // set debug mode
#ifdef BUILD_TYPE
  bool debug = set_envvar(BUILD_TYPE);
  if (debug)
    log(INFO, 200, "Debug mode set", NULL);
#endif
  min_log_level = get_min_log_level();

  // main process
  OS os;
  process p = {
      .name = "ChicOS", .pid = (i32)getpid(), .child = NULL, .status = READY};
  os.process_list[0] = p;
  log_process(os.process_list[0].pid, os.process_list);

  return 0;
}
