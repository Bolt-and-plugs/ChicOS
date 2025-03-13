#include "chicos.h"
#include "modules/process/process.h"
#include "modules/schedduler/schedduler.h"

log_level min_log_level;
bool debug;
process process_list[10];

bool set_envvar(const char *mode) {
  if (strcmp(mode, "Debug") == 0 || strcmp(mode, "DEBUG") == 0) {
    debug = true;
    return true;
  }

  debug = false;
  return false;
}

void handle_args(int argc, char **argv) {
  // printing tags if tags
  for (int i = 1; i < argc; i += 2) {
    if (argv[i][0] == '-' && strlen(argv[i]) > 1 && argv[i][2] != '-')
      printf("%s ", argv[i]);
  }
  puts("");

  // printing values
  for (int i = 2; i < argc; i += 2) {
    printf("%s ", argv[i]);
  }
  puts("");
}

int main(int argc, char **argv) {
  // handle_args(argc, argv);
  //  set debug mode
#ifdef BUILD_TYPE
  bool debug = set_envvar(BUILD_TYPE);
  if (debug)
    log(INFO, 200, "Debug mode set", NULL);
#endif
  min_log_level = get_min_log_level();

  // main process
  process p = {.name = "ChicOS",
               .pid = (i32)getpid(),
               .child = NULL,
               .status = READY,
               .tickets = 50};
  process_list[0] = p;
  process p2 = {.name = "processo diferente",
                .pid = (i32)getpid() + 1,
                .child = NULL,
                .status = READY,
                .tickets = 30};
  process_list[1] = p2;
  process p3 = {.name = "processo 95",
                .pid = (i32)getpid() + 2,
                .child = NULL,
                .status = READY,
                .tickets = 10};
  process_list[2] = p3;

  for (int i = 0; i < 10; i++) {
    log_process(get_winner());
    sleep(1);
  }

  return 0;
}
