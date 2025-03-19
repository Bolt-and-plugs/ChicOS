#include "chicos.h"
#include "modules/log/log.h"
#include "modules/schedduler/schedduler.h"
#include "modules/utils/utils.h"

log_level min_log_level;
bool debug;
process process_list[10];
App app;

bool set_envvar(const char *mode) {
  if (strcmp(mode, "Debug") == 0 || strcmp(mode, "DEBUG") == 0) {
    debug = true;
    return true;
  }

  debug = false;
  return false;
}

void init_app(int system_size) {
  // app = (App){.virtua_mem = create_arena(system_size)};
}

void handle_args(int *args, int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && strlen(argv[i]) > 1 && argv[i][2] != '-')
      printf("%s ", argv[i]);
    char *str_arg = argv[i];
    if (strcmp(str_arg, "--help") == 0 || strcmp(str_arg, "-h") == 0) {
      args[0] = 1;
    }
    if (strcmp(str_arg, "--sys-len") == 0 || strcmp(str_arg, "-sl") == 0) {
      int val = parse_string_to_int(argv[i + 1]);
      if (val == 0 || !valid_int(val)) {
        c_log(ERROR, MEM_STATUS, "Bad system length", NULL);
        exit(0);
      }
      args[1] = val;
    }
  }
  puts("");
}

int main(int argc, char **argv) {
  int args[argc];
  if (argc > 1) {
    handle_args(args, argc, argv);
    if (args[0] == 1) {
      puts("Help thing");
      return 0;
    }
    if (valid_int(args[1])) {
      printf("Size of system being set to %d\n", args[1]);
      // init_app(args[1]);
    }
  }

  //  set debug mode
#ifdef BUILD_TYPE
  bool debug = set_envvar(BUILD_TYPE);
  if (debug)
    c_log(INFO, 200, "Debug mode set", NULL);
#endif
  min_log_level = get_min_log_level();

  char *process_name[5] = {"ChicOS", "2kdgajkgad", "aiagkj3", "4sim", "TESTE"};
  // main process
  for (int i = 0; i < 5; i++) {
    process_list[i] = (process){.name = process_name[i],
                                .pid = (i32)getpid() + i,
                                .child = NULL,
                                .status = READY,
                                .tickets = 100 / 5};
  }

  for (int i = 0; i < 10; i++) {
    log_process(get_winner_proc());
    sleep(1);
  }

  Arena *a = create_arena(64);

  return 0;
}
