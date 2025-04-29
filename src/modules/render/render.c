#include "render.h"
#include "../../chicos.h"
#include "../memory/mem.h"
#include <unistd.h>

extern App app;

void print_logo(WINDOW *win) {
  wclear(win);
  printw("                                      @@@@@  @     @@@@              "
         "                \n");
  printw("                            @@@@    @@@@@@@@@@  @@@@@@@@@@           "
         " @              \n");
  printw("                      @@@   @@@    @@@          @@@     @@@          "
         "   @@           \n");
  printw("                   @@@@@     @@@   @@@         @@@@      @@@         "
         "    @@@@        \n");
  printw("              @@     @@@@    @@@@  @@@@      @  @@@     @@@@      @  "
         "   @@@@@@@@     \n");
  printw("           @@@@@      @@@@    @@@   @@@@@@@@@@   @@@@@@@@@@       @  "
         "    @@@@@@@@@   \n");
  printw("        @     @@@ @@@@@@@@    @@@@      @@@          @@@         @   "
         "     @@@@@@@@@@ \n");
  printw("       @@@    @@@@@     @@@   @@@  @@@@@@@@@                 @@  @@  "
         "       @@@@@@@@@\n");
  printw("    @@@@@@@    @@@@     @@@@   @@           @ @@      @@@ @@@@@@@    "
         "         @@@@@@@\n");
  printw("   @@@          @@@@         @          @@@ @@@@@@@ @  @@@@@@@@ @    "
         "           @@@@@\n");
  napms(400);
  wrefresh(win);
  printw("  @@@        @   @@@        @      @@@ @@@@@@@@@@@@@@@@ @            "
         "             @@@\n");
  printw("  @@@       @@@            @     @  @@@@@@@  @@@@@@@@@@@@@@@ @       "
         "               @\n");
  printw("  @@@@@   @@@@            @  @@ @@@@@@@ @@@@@ @@@@@@@@  @@@@@@@ @   "
         "@@               \n");
  printw("   @@@@@@@@@      @@      @  @@@@@@@@@@  @@@    @@@@@@@  @ "
         "@@@@@@@@@@                \n");
  wrefresh(win);
  napms(400);
  printw("                         @ @@ @@@@@@@@@@@@@@@   @@@@@@@@    @@@@ @@  "
         "                \n");
  printw("           @@  @           @@    @@@      @@@@@@@@@@@ @@             "
         "                 \n");
  printw("         @    @@ @  @@ @    @@  @ @@  @@@@@@@@@@@@@  @@@@ @@@@       "
         "                \n");
  printw("         @ @ @@  @ @@@     @@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@  @@ @    "
         "                \n");
  wrefresh(win);
  napms(400);
  printw("         @ @@   @ @@@@  @@ @@@ @@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@  "
         "                \n");
  printw("         @ @@    @@@@ @ @@  @@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@ @@@ "
         "@                \n");
  printw("         @   @  @@       @   @@@@@@@@@@@@@@@@@@ @@ @ @@@      @@@  "
         "@@ @              \n");
  printw("        @      @ @   @ @@ @    @@@ @@@@@@@@@ @@@@@@ @@@         @@@ "
         "@@@@             \n");
  wrefresh(win);
  napms(400);
  printw("         @@    @ @@@@@     @    @@@@@@@@@@@@@ @ @@ @@@@@         @@@ "
         "@@@@            \n");
  printw("      @    @@ @@@@@@@  @ @     @@@@@@@@ @@ @@@ @  @@@ @@@       "
         "@@@@@@@@ @           \n");
  printw("            @@@@ @    @@      @    @@@@@@@@@ @@@ @@ @@@@@@@@@     @ "
         "@@@@@ @          \n");
  printw("      @ @@  @@@@@@    @@     @   @@ @@ @@  @@@@ @@    @@@@@@        "
         "@@@ @@@          \n");
  wrefresh(win);
  napms(400);
  printw("       @@@@  @  @@  @ @        @@ @  @ @@@@ @@@@@      @@@@ @       "
         "@@ @@@           \n");
  printw("        @ @     @@@@@ @     @  @  @@@@@ @@@ @@@@       @ @@@ @@    "
         "@@ @    @         \n");
  printw("          @  @@@@@@ @  @   @   @@@  @@  @@@@@@@        @@@@@@@@@@@ "
         "@@@@@@@@@@        \n");
  printw("           @@@@@@@@@@ @@@ @    @@@@ @@@@@@@@@@       @@ @@@@@@@@ @@  "
         "@@@@@@@@@       \n");
  wrefresh(win);
  napms(400);
  printw("            @@@@@ @@@ @   @@@ @@@    @@ @@@ @@@@@ @@@@  @@ "
         "@@@@@@@@@@@@@             \n");
  printw("               @@@ @@     @@@@@@@@@   @ @@@@@@@        @@@@@@@@@     "
         "@  @@@@         \n");
  printw("                @@@@      @ @@@@@@@@@@@@@@@@@@@ @ @   @     @@    "
         "@@@    @@@@        \n");
  printw("                 @@@@@   @@@@@@@@@@@     @ @@@@      @@@@ @@@@    @@ "
         "   @@@@@@       \n");
  wrefresh(win);
  napms(400);
  printw("                  @@@@@@@ @@@@@@ @@             @@@@@   @   @@@   @@ "
         "   @@  @@@      \n");
  printw("                   @@@  @@@ @@@       @@@@@@@@   @@@   @    @@@@ @@@ "
         "   @@@@@@@@     \n");
  printw("                  @@@@@@@@@@     @@  @@@    @@@   @@@@@@@     @@@@@@ "
         "  @@@    @@@    \n");
  printw("                        @ @@@@@@@@@  @@@    @@@   @@@   @  @   @@@@@ "
         "  @@@    @@@@   \n");
  printw("                        @@@@ @@@      @@@@@@@@     @@@    @@   @@@@@ "
         "                \n");
  printw("                         @   @@@      @@@  @@@@    @@@@@@@@@         "
         "                \n");
  printw("                             @@@@      @@@   @@@@ @@                 "
         "                \n");
  printw("                              @@@      @@@@                          "
         "                 \n");
  printw("                              @@@@    @@                             "
         "                 \n");
  printw("                               @@@@                                  "
         "                \n");
  napms(3000);
  wclear(win);
  wrefresh(win);
}

void print_welcome(WINDOW *win) {
  wclear(win);
  wprintw(win, "\nBEM-VINDO\n");
  wrefresh(win);
  napms(3000);
}

void render_loop(WINDOW *win) {
  while (!app.loop_stop) {
    wclear(win);
    wprintw(win, "free mem: %2.2f%%", retrieve_free_mem_percentage());
    wprintw(win, "used mem: %2.2f%%", retrieve_used_mem_percentage());
    wprintw(win, "quantum time: %ld", app.cpu.quantum_time);
    wrefresh(win);
    napms(10);
  }
}
WINDOW *create_newwin(int height, int width, int starty, int startx) {
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0); /* 0, 0 gives default characters
                         * for the vertical and horizontal
                         * lines */
  wrefresh(local_win);  /* Show that box 		*/

  return local_win;
}

void *init_render(void *arg) {
  WINDOW *w1, *w2, *w3;
  int startx, starty, width, height;
  int ch;
  initscr();
  cbreak();
  noecho();
  curs_set(0);

  height = 3;
  width = 10;
  starty = (LINES - height) / 2;
  startx = (COLS - width) / 2;
  printw("ChicOS(S) | ");
  if (app.debug)
    printw("Debug Mode Set | ");
  printw("Press Ctrl+c to exit");
  refresh();
  w1 = create_newwin(height/2, width/2, starty/2, 0);
  w2 = create_newwin(height/2, width/2, starty/2, startx);
  w3 = create_newwin(height/2, width, starty, 0);

  if (!app.debug) {
    print_logo(w1);
    print_welcome(w1);
  }

  render_loop(w1);

  endwin();
  return NULL;
}
