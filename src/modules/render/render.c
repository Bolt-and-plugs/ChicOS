#include "render.h"
#include "../../chicos.h"
#include "../memory/mem.h"
#include <locale.h>
#include <unistd.h>

extern App app;
volatile sig_atomic_t resized = 0;

void handle_resize(int sig) { resized = 1; }

void print_welcome(WINDOW *win) {
  int y, x;
  getmaxyx(win, y, x);
  wclear(win);
  box(win, 0, 0);
  mvwprintw(win, y / 2 - 1, ((x - 10) / 2) - 1, "[ CHIC✪S ]");
  mvwprintw(win, y / 2, (x - 10) / 2, "BEM-VINDO");
  wprintw(win, "");
  wrefresh(win);
  napms(3000);
}

void print_goodbye(WINDOW *win) {
  int y, x;
  getmaxyx(win, y, x);
  wclear(win);
  box(win, 0, 0);
  mvwprintw(win, y / 2, (x - 10) / 2, "ATÉ MAIS!");
  wprintw(win, "");
  wrefresh(win);
  napms(3000);
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0);
  wrefresh(local_win);
  return local_win;
}

void status_bar(WINDOW *status_win) {
  // Status bar
  mvwprintw(status_win, 0, 0, "ChicOS(S) | %s Press CTRL+C to quit",
            app.debug ? "Debug Mode | " : "");
  wrefresh(status_win);
}

void clear_renderer() {
  wclear(app.rdr.status_win);
  wclear(app.rdr.left_panel);
  wclear(app.rdr.right_top);
  wclear(app.rdr.right_bottom);
  delwin(app.rdr.status_win);
  delwin(app.rdr.left_panel);
  delwin(app.rdr.right_top);
  delwin(app.rdr.right_bottom);
}

void init_renderer() {
  app.rdr.status_win = create_newwin(1, COLS, 0, 0);
  app.rdr.left_panel = create_newwin(LINES - 1, COLS / 2, 1, 0);
  app.rdr.right_top = create_newwin((LINES - 1) / 2, COLS / 2, 1, COLS / 2);
  app.rdr.right_bottom =
      create_newwin((LINES - 1) / 2, COLS / 2, 1 + (LINES - 1) / 2, COLS / 2);
}

void render_left_panel() {
  wclear(app.rdr.left_panel);
  int max_cols, max_rows;
  getmaxyx(app.rdr.left_panel, max_cols, max_rows);
  box(app.rdr.left_panel, 0, 0);
  wprintw(app.rdr.left_panel, " CPU - quantum time: %ld ",
          app.cpu.quantum_time);
  wrefresh(app.rdr.left_panel);
}

void render_right_top() {
  wclear(app.rdr.right_top);
  int max_cols, max_rows;
  getmaxyx(app.rdr.right_top, max_cols, max_rows);
  box(app.rdr.right_top, 0, 0);
  int bar_width = max_cols - 1;
  float used_percent = retrieve_used_mem_percentage();

  char bar[bar_width + 1];
  int filled = (int)(used_percent * bar_width / 100.0);
  filled = filled > bar_width ? bar_width : filled;

  memset(bar, '=', filled);
  memset(bar + filled, '-', bar_width - filled);
  bar[bar_width] = '\0';

  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);

  wprintw(app.rdr.right_top, "Memory");
  mvwprintw(app.rdr.right_top, 1, 1, "Memory Usage:");
  mvwprintw(app.rdr.right_top, 2, 1, "[%.*s] %2.1f%%", bar_width, bar,
            used_percent);

  if (used_percent > 80) {
    wattron(app.rdr.right_top, COLOR_PAIR(1));
  } else {
    wattron(app.rdr.right_top, COLOR_PAIR(2));
  }

  mvwprintw(app.rdr.right_top, 2, 2, "%.*s", filled, bar);
  wattroff(app.rdr.right_top, COLOR_PAIR(1) | COLOR_PAIR(2));

  mvwprintw(app.rdr.right_top, 4, 1, "Used:  %2.2f%%", used_percent);
  mvwprintw(app.rdr.right_top, 5, 1, "Free:  %2.2f%%",
            retrieve_free_mem_percentage());

  wrefresh(app.rdr.right_top);
  napms(100);

  wrefresh(app.rdr.right_top);
  napms(100);
}

void render_right_bottom() {
  // right bottom
  wclear(app.rdr.right_bottom);
  int max_cols, max_rows;
  getmaxyx(app.rdr.right_bottom, max_cols, max_rows);
  box(app.rdr.right_bottom, 0, 0);
  wprintw(app.rdr.right_bottom, "User Input ");
  wrefresh(app.rdr.right_bottom);
  napms(100);
}

void render_loop() {
  status_bar(app.rdr.status_win);
  nodelay(app.rdr.left_panel, TRUE);
  nodelay(app.rdr.right_top, TRUE);
  nodelay(app.rdr.right_bottom, TRUE);

  while (!app.loop_stop) {
    if (resized) {
      resized = 0;
      endwin();
      refresh();

      clear_renderer();
      init_renderer();
    }
    render_left_panel();
    render_right_top();
    render_right_bottom();
  }
}

void *init_render(void *arg) {
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  curs_set(0);

  if (!app.debug) {
    WINDOW *logo_window = create_newwin(LINES, COLS, 0, 0);
    print_welcome(logo_window);
  }

  init_renderer();
  render_loop();
  clear_renderer();

  if (!app.debug) {
    WINDOW *goodbye = create_newwin(LINES, COLS, 0, 0);
  }

  endwin();
  return NULL;
}
