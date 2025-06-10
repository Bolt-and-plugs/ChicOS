#ifndef _RENDER
#define _RENDER

#include "../../defines.h"
#include "ncurses.h"


typedef struct __renderer {
  bool active;
  WINDOW *status_win, *left_panel, *right_top, *right_bottom, *left_bottom;
  char* output_buff;
  sem_t renderer_s;
} renderer;

void print_logo(WINDOW *win);
void *init_render(void* arg);
void render_log(char *statement);

#endif
