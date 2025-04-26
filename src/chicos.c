#include "chicos.h"
#include "modules/cpu/cpu.h"
#include "modules/log/log.h"
#include "modules/memory/mem.h"
#include "modules/process/process.h"
#include "modules/render/render.h"
#include "modules/scheduler/scheduler.h"
#include "modules/utils/utils.h"

log_level min_log_level;
bool debug;
App app;

void handle_signal(int signal) {
  app.loop_stop = 1; 
  printf("Stopping loops\n");
}

bool set_envvar(const char *mode) {
  if (strcmp(mode, "Debug") == 0 || strcmp(mode, "DEBUG") == 0) {
    debug = true;
    return true;
  }

  debug = false;
  return false;
}

void init_app(int mem_size) {
  init_mem(mem_size);
  init_pcb();

  signal(SIGINT, handle_signal);

  if (pthread_create(&app.cpu.cpu_t, NULL, init_cpu, NULL) != 0) {
    perror("Failed to create CPU thread");
    exit(1);
  }

  if (pthread_create(&app.mem->render_t, NULL, init_render,
                     debug ? "no_logo" : "logo") != 0) {
    perror("Failed to create render thread");
    exit(1);
  }
}

void clear_app() {
  pthread_join(app.cpu.cpu_t, NULL);
  pthread_join(app.mem->render_t, NULL);
  clear_pcb();
  clear_mem();
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

  set_debug_mode();
  // main loop
  init_app(mem_size);
  clear_app();
  return 0;
}
