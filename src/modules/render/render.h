#ifndef _RENDER
#define _RENDER

#include "../../defines.h"
#ifndef __linux__
#include "curses.h"
#else
#include "ncurses.h"
#endif

typedef struct __renderer {
  bool active;
  WINDOW *status_win, *left_panel, *right_top, *right_mid, *right_bottom,
      *left_bottom;
  char *output_buff;
  sem_t renderer_s;
} renderer;

void print_logo(WINDOW *win);
void *init_render(void *arg);
void render_log(char *statement);

#endif
