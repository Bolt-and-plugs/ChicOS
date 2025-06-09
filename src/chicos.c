#include "chicos.h"
#include "modules/cpu/cpu.h"
#include "modules/log/log.h"
#include "modules/memory/mem.h"
#include "modules/process/process.h"
#include "modules/render/render.h"
#include "modules/utils/utils.h"

App app;

void handle_signal(sig_atomic_t signal) {
  app.loop_stop = 1;
  c_info("Stopping loops");
}

bool set_envvar(const char *mode) {
  if (strcmp(mode, "Debug") == 0 || strcmp(mode, "DEBUG") == 0) {
    app.debug = true;
    return true;
  }

  app.debug = false;
  return false;
}

void init_app(int mem_size, bool should_render) {
  srand(time(NULL));
  init_mem(mem_size);
  init_pcb();
  init_semaphore_list();

  signal(SIGINT, handle_signal);

  if (pthread_create(&app.cpu.cpu_t, NULL, init_cpu, NULL) != 0) {
    c_crit_error(THREAD_INIT_ERROR, "Failed to create CPU thread");
  }

  if (pthread_create(&app.disk.disk_t, NULL, init_disk, NULL) != 0) {
    c_crit_error(THREAD_INIT_ERROR, "Failed to create DISK thread");
  }

  if (!should_render) {
    c_info("Disabled UI");
    return;
  }

  if (pthread_create(&app.mem->render_t, NULL, init_render, NULL) != 0) {
    c_crit_error(THREAD_INIT_ERROR, "Failed to create render thread");
  }
}

void clear_app(bool should_render) {
  pthread_join(app.cpu.cpu_t, NULL);
  pthread_join(app.disk.disk_t, NULL);
  if (should_render)
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
  app.min_log_level = get_min_log_level();
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
      int val = atoi(argv[i + 1]);
      if (val <= 0 || val >= 4 * MB || !valid_int(val)) {
        c_crit_error(MEM_ERROR, "Bad system length");
      }
      args[1] = val;
    }

    if (strcmp(str_arg, "--no_render") == 0 || strcmp(str_arg, "-nr") == 0) {
      args[2] = true;
    }
  }

  // print help
  if (args[0] == HELP) {
    puts("Valid Arguments:");
    puts("\t-ms, --mem-size -> (integer) Memory size in bytes");
    puts("\t-nr, --no_render -> (store_true / bool) Disables UI rendering");
    exit(-1);
  }
  puts("");
}

int main(int argc, char **argv) {
  i32 args[10];
  i32 mem_size = 1 * MB;
  bool should_render = true;

  if (argc > 1) {
    handle_args(args, argc, argv);
    if (args[1] && valid_int(args[1]))
      mem_size = args[1];
    if (args[2] && valid_int(args[2]))
      should_render = false;
  }

  set_debug_mode();
  // main loop
  init_app(mem_size, should_render);
  clear_app(should_render);
  return 0;
}
