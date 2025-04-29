#ifndef _RENDER
#define _RENDER

#include "../../defines.h"
#include "curses.h"


typedef struct __renderer {
  WINDOW *status_win, *left_panel, *right_top, *right_bottom;
} renderer;

void print_logo(WINDOW *win);
void *init_render(void* arg);

#endif
