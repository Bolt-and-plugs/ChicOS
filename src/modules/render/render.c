#include "render.h"
#include "../../chicos.h"
#include "../memory/mem.h"
#include <curses.h>
#include <unistd.h>

extern App app;

void print_logo() {
  clear();
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
  refresh();
  printw("  @@@        @   @@@        @      @@@ @@@@@@@@@@@@@@@@ @            "
         "             @@@\n");
  printw("  @@@       @@@            @     @  @@@@@@@  @@@@@@@@@@@@@@@ @       "
         "               @\n");
  printw("  @@@@@   @@@@            @  @@ @@@@@@@ @@@@@ @@@@@@@@  @@@@@@@ @   "
         "@@               \n");
  printw("   @@@@@@@@@      @@      @  @@@@@@@@@@  @@@    @@@@@@@  @ "
         "@@@@@@@@@@                \n");
  refresh();
  napms(400);
  printw("                         @ @@ @@@@@@@@@@@@@@@   @@@@@@@@    @@@@ @@  "
         "                \n");
  printw("           @@  @           @@    @@@      @@@@@@@@@@@ @@             "
         "                 \n");
  printw("         @    @@ @  @@ @    @@  @ @@  @@@@@@@@@@@@@  @@@@ @@@@       "
         "                \n");
  printw("         @ @ @@  @ @@@     @@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@  @@ @    "
         "                \n");
  refresh();
  napms(400);
  printw("         @ @@   @ @@@@  @@ @@@ @@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@  "
         "                \n");
  printw("         @ @@    @@@@ @ @@  @@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@ @@@ "
         "@                \n");
  printw("         @   @  @@       @   @@@@@@@@@@@@@@@@@@ @@ @ @@@      @@@  "
         "@@ @              \n");
  printw("        @      @ @   @ @@ @    @@@ @@@@@@@@@ @@@@@@ @@@         @@@ "
         "@@@@             \n");
  refresh();
  napms(400);
  printw("         @@    @ @@@@@     @    @@@@@@@@@@@@@ @ @@ @@@@@         @@@ "
         "@@@@            \n");
  printw("      @    @@ @@@@@@@  @ @     @@@@@@@@ @@ @@@ @  @@@ @@@       "
         "@@@@@@@@ @           \n");
  printw("            @@@@ @    @@      @    @@@@@@@@@ @@@ @@ @@@@@@@@@     @ "
         "@@@@@ @          \n");
  printw("      @ @@  @@@@@@    @@     @   @@ @@ @@  @@@@ @@    @@@@@@        "
         "@@@ @@@          \n");
  refresh();
  napms(400);
  printw("       @@@@  @  @@  @ @        @@ @  @ @@@@ @@@@@      @@@@ @       "
         "@@ @@@           \n");
  printw("        @ @     @@@@@ @     @  @  @@@@@ @@@ @@@@       @ @@@ @@    "
         "@@ @    @         \n");
  printw("          @  @@@@@@ @  @   @   @@@  @@  @@@@@@@        @@@@@@@@@@@ "
         "@@@@@@@@@@        \n");
  printw("           @@@@@@@@@@ @@@ @    @@@@ @@@@@@@@@@       @@ @@@@@@@@ @@  "
         "@@@@@@@@@       \n");
  refresh();
  napms(400);
  printw("            @@@@@ @@@ @   @@@ @@@    @@ @@@ @@@@@ @@@@  @@ "
         "@@@@@@@@@@@@@             \n");
  printw("               @@@ @@     @@@@@@@@@   @ @@@@@@@        @@@@@@@@@     "
         "@  @@@@         \n");
  printw("                @@@@      @ @@@@@@@@@@@@@@@@@@@ @ @   @     @@    "
         "@@@    @@@@        \n");
  printw("                 @@@@@   @@@@@@@@@@@     @ @@@@      @@@@ @@@@    @@ "
         "   @@@@@@       \n");
  refresh();
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
  clear();
  refresh();
}

void print_welcome() {
  clear();
  printw("\nBEM-VINDO\n");
  refresh();
  napms(3000);
  clear();
  refresh();
}

void render_loop() {
  while (!app.loop_stop) {
    clear();
    mvprintw(0, 0, "free mem: %2.2f%%", retrieve_free_mem_percentage());
    mvprintw(1, 0, "used mem: %2.2f%%", retrieve_used_mem_percentage());
    mvprintw(2, 0, "quantum time: %ld", app.cpu.quantum_time);
    if (app.debug)
      mvprintw(3, 0, "Debug Mode Set");
    refresh();
    napms(10);
  }
}

void *init_render(void *arg) {
  initscr();
  cbreak();
  noecho();
  curs_set(0);

  if (!app.debug) {
    print_logo();
    print_welcome();
  }

  render_loop();

  endwin();
  return NULL;
}
