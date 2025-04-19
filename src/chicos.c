#include "chicos.h"
#include "modules/io/file.h"
#include "modules/log/log.h"
#include "modules/memory/mem.h"
#include "modules/schedduler/schedduler.h"
#include "modules/user/user.h"
#include "modules/utils/utils.h"

log_level min_log_level;
bool debug;
App app;

bool set_envvar(const char *mode) {
  if (strcmp(mode, "Debug") == 0 || strcmp(mode, "DEBUG") == 0) {
    debug = true;
    return true;
  }

  debug = false;
  return false;
}

void init_app(int mem_size) {
  printf("Size of system being set to %d\n", mem_size);
  init_mem(mem_size);
  app.quantum_time = 0; // ms
  init_pcb();
  // init_cpu()
}

void set_debug_mode() {
  //  set debug mode
#ifdef BUILD_TYPE
  bool debug = set_envvar(BUILD_TYPE);
  if (debug)
    c_info("Debug mode set");
#endif
  min_log_level = get_min_log_level();
}

void handle_args(int *args, int argc, char **argv) {
  char *str_arg;

  for (int i = 1; i < argc; i++) {
    str_arg = argv[i];

    if (strcmp(str_arg, "--help") == 0 || strcmp(str_arg, "-h") == 0) {
      args[0] = HELP;
      break;
    }

    // sets memory size on args
    if (strcmp(str_arg, "--mem-size") == 0 || strcmp(str_arg, "-ms") == 0) {
      int val = parse_string_to_int(argv[i + 1]);
      if (val <= 0 || val >= 4 * MB || !valid_int(val)) {
        c_error(MEM_ERROR, "Bad system length");
        exit(0);
      }
      args[1] = val;
    }
  }

  // print help
  if (args[0] == HELP) {
    puts("Valid Arguments:");
    puts("--mem-size -> (integer) Memory size in bytes");
    exit(-1);
  }

  puts("");
}

int main(int argc, char **argv) {
  i32 args[argc];
  i32 mem_size = 1 * MB;

  if (argc > 1) {
    handle_args(args, argc, argv);

    if (args[1] && valid_int(args[1]))
      mem_size = args[1];
  }

  init_app(mem_size);
  // print_logo();
  set_debug_mode();

  int *teste = alloc(sizeof(int));
  printf("free mem: %2.f\n", retrieve_free_mem_percentage());
  printf("used mem: %2.f\n", retrieve_used_mem_percentage());
  dealloc(teste);

  return 0;
}
