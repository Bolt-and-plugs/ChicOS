#include "render.h"
#include "../../chicos.h"
#include "../memory/mem.h"
#include "../user/user.h"
#include <ctype.h>
#include <locale.h>
#include <unistd.h>

extern App app;
volatile sig_atomic_t resized = 0;

void handle_resize(int sig) { resized = 1; }

user get_credentials(WINDOW *win) {
  user login = {0};
  int y, x;
  getmaxyx(win, y, x);
  int form_width = 40;
  int start_x = (x - form_width) / 2;

  // Enable special characters and input
  keypad(win, TRUE);
  curs_set(1);
  noecho();

  // Draw form frame
  werase(win);
  box(win, 0, 0);
  mvwprintw(win, 3, (x - 75) / 2, " ____       ____     __  __  ______   ____     _____   ____         ____     ");
  mvwprintw(win, 4, (x - 75) / 2, "/\\  _\\     /\\  _`\\  /\\ \\/\\ \\/\\__  _\\ /\\  _`\\  /\\  __`\\/\\  _`\\      /\\__ \\    ");
  mvwprintw(win, 5, (x - 75) / 2, "\\ \\ \\/     \\ \\ \\/\\_\\\\ \\ \\_\\ \\_/_/\\ \\/ \\ \\ \\/\\_\\\\ \\ \\/\\ \\ \\,\\L\\_\\    \\/_/\\ \\   ");
  mvwprintw(win, 6, (x - 75) / 2, " \\ \\ \\      \\ \\ \\/_/_\\ \\  _  \\ \\ \\ \\  \\ \\ \\/_/_\\ \\ \\ \\ \\/_\\__ \\       \\ \\ \\  ");
  mvwprintw(win, 7, (x - 75) / 2, "  \\ \\ \\_     \\ \\ \\L\\ \\\\ \\ \\ \\ \\ \\_\\ \\__\\ \\ \\L\\ \\\\ \\ \\_\\ \\/\\ \\L\\ \\      \\_\\ \\ ");
  mvwprintw(win, 8, (x - 75) / 2, "   \\ \\___\\    \\ \\____/ \\ \\_\\ \\_\\/\\_____\\\\ \\____/ \\ \\_____\\ `\\____\\     /\\___\\");
  mvwprintw(win, 9, (x - 75) / 2, "    \\/___/     \\/___/   \\/_/\\/_/\\/_____/ \\/___/   \\/_____/\\/_____/     \\/___/");
  wrefresh(win);

  // Username Field
  mvwprintw(win, y / 2 - 2, start_x, "Username:");
  WINDOW *uname_win = derwin(win, 3, form_width - 10, y / 2 - 1, start_x);
  keypad(uname_win, TRUE);
  box(uname_win, 0, 0);
  wrefresh(uname_win);
  wrefresh(win);

  // Password Field (positioned below username)
  mvwprintw(win, y / 2 + 2, start_x, "Password:");
  WINDOW *pass_win = derwin(win, 3, form_width - 10, y / 2 + 3, start_x);
  keypad(pass_win, TRUE);
  box(pass_win, 0, 0);
  wrefresh(pass_win);
  wtimeout(pass_win, 100);
  wrefresh(win);

  // Get username
  echo();
  mvwgetnstr(uname_win, 1, 1, login.username, sizeof(login.username) - 1);
  noecho();

  // Get password with masking
  int pos = 0;
  int ch;
  wmove(pass_win, 1, 1);
  wrefresh(pass_win);

  while (1) {
    ch = wgetch(pass_win);
    if (ch == ERR)
      continue;
    if (ch == '\n')
      break;
    if ((ch == KEY_BACKSPACE || ch == 127 || ch == 8) && pos > 0) {
      pos--;
      mvwaddch(pass_win, 1, pos + 1, ' ');
      wmove(pass_win, 1, pos + 1);
    } else if (isprint(ch) && pos < (int)sizeof(login.password) - 1) {
      login.password[pos] = ch;
      mvwaddch(pass_win, 1, ++pos, '*');
    }
    wrefresh(pass_win);
    wrefresh(win);
  }
  login.password[pos] = '\0';

  // Cleanup
  delwin(uname_win);
  delwin(pass_win);
  curs_set(0);
  return login;
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
  if (!app.debug) {
    WINDOW *goodbye = create_newwin(LINES, COLS, 0, 0);
    print_goodbye(goodbye);
  }
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
  // int max_cols, max_rows;
  // getmaxyx(app.rdr.left_panel, max_cols, max_rows);
  box(app.rdr.left_panel, 0, 0);
  mvwprintw(app.rdr.left_panel, 1, 1, " CPU - quantum time: %ld ", app.cpu.quantum_time);
  wrefresh(app.rdr.left_panel);
}

void render_right_top() {
  wclear(app.rdr.right_top);
  int max_cols, max_rows;
  getmaxyx(app.rdr.right_top, max_rows, max_cols);
  box(app.rdr.right_top, 0, 0);
  int bar_width = max_cols - 4;
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
  mvwprintw(app.rdr.right_top, 2, 1, "[%.*s]", bar_width, bar);

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
  wclear(app.rdr.right_bottom);
  // int max_cols, max_rows;
  // getmaxyx(app.rdr.right_bottom, max_cols, max_rows);
  box(app.rdr.right_bottom, 0, 0);
  wprintw(app.rdr.right_bottom, "User Input ");
  wrefresh(app.rdr.right_bottom);
  wprintw(app.rdr.right_bottom,
          "                                                  \n");
  wprintw(app.rdr.right_bottom,
          "                      █████   ███                 \n");
  wprintw(app.rdr.right_bottom,
          "              █ ██   ██   █ ██   ██ █     █       \n");
  wprintw(app.rdr.right_bottom,
          "         █  ██   ██ ██      ██   ██        ██▓    \n");
  wprintw(app.rdr.right_bottom,
          "      ▓███  ███  ▓█   █████  █████     █    ███▓  \n");
  wprintw(app.rdr.right_bottom,
          "    ██  ████  ██  ██ █████       ████▓██      ███▓\n");
  wprintw(app.rdr.right_bottom,
          "  ██     ██   █   ░░▒▒▒▒▓▓███  ████████         ██\n");
  wprintw(app.rdr.right_bottom,
          " ██    ██ █     █▒▒▒▓█░████████████               \n");
  wprintw(app.rdr.right_bottom,
          " ███  ██       █▓█░█▒█████░█████ ▓███▓  █         \n");
  wprintw(app.rdr.right_bottom,
          "   ███         ░███████░░████▒▒▒█ █▓████          \n");
  wprintw(app.rdr.right_bottom,
          "      █▒▒██░█  ██░▓█▒▓████████▒▒██       █        \n");
  wprintw(app.rdr.right_bottom,
          "     ██▓█▓█▒▒▒░ ▒███░████████▒████▓░█▒            \n");
  wprintw(app.rdr.right_bottom,
          "     █▓█▓▒▒▒░▓█ █████░█████████▓█████░██          \n");
  wprintw(app.rdr.right_bottom,
          "     ░▒▒█▒█▓█▓██ █▓████████▓█▓▓█▓▒░░░█▒░██        \n");
  wprintw(app.rdr.right_bottom,
          "    ██▓▒▒▓██▓▒▒░   ▒██████░██▓██▓▒▒▒▒▒██▓█▓       \n");
  wprintw(app.rdr.right_bottom,
          "   █▒▒▒▓███▒▒▒   █░█▓█████▒▓░█▓▒▓█▓▓▒▒▒░█▒██      \n");
  wprintw(app.rdr.right_bottom,
          "    ▒█▓▓██▓▒██   ░▒░█░░█▒▒█▒█▒▒▒███▓▒▒▒▒████      \n");
  wprintw(app.rdr.right_bottom,
          "     ██▒▒▓██▒█  █░▓▓▓▒▓▒█▓██▒▒▒▒▓░███▓▒█▒▓▓▓█     \n");
  wprintw(app.rdr.right_bottom,
          "       ███▓█░▒██▒▒▓▒▒▓█▓▓▓▓▒▒▒▒█▒███████▓▓████    \n");
  wprintw(app.rdr.right_bottom,
          "       ▓███▒█▒░▓▓███▒▒▓█▓▓███░▒▒▒▓▓████   ███     \n");
  wprintw(app.rdr.right_bottom,
          "         ▓█▓▒▒▒██████▒░▒▒█▒█▒▓▒  ███   █   ███    \n");
  wprintw(app.rdr.right_bottom,
          "          ▒████▓█████▒▒▒    ███  █ ██ ███ ██ ██   \n");
  wprintw(app.rdr.right_bottom,
          "           ██████    ███ ███ ████   ████  ██  ██  \n");
  wprintw(app.rdr.right_bottom,
          "              █████ █ ██▓██   █   █  ███ ███      \n");
  wprintw(app.rdr.right_bottom,
          "              █  ███   ██ ██  ████                \n");
  wprintw(app.rdr.right_bottom,
          "                  ██   ███                        \n");
  wprintw(app.rdr.right_bottom,
          "                  ███                             \n");
  napms(100);
}

void render_loop() {
  status_bar(app.rdr.status_win);
  nodelay(app.rdr.left_panel, TRUE);
  nodelay(app.rdr.right_top, TRUE);
  nodelay(app.rdr.right_bottom, TRUE);

  while (!app.loop_stop) {
    napms(100);
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

  // Initialize colors
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);

  // Create and display login window
  //WINDOW *logo_window = create_newwin(LINES, COLS, 0, 0);
  //user login = get_credentials(logo_window);
  //app.user = read_login_data(login);

  //// Display username (debug/test line)
  //mvwprintw(logo_window, 4, 4, app.user->username);
  //wrefresh(logo_window);

  //// Handle new user creation if no existing user
  //if (!app.user) {
  //  write_login_data(login);
  //  app.user = read_login_data(login);
  //  if (!app.user) {
  //    c_crit_error(INVALID_INPUT, "Could not login");
  //    endwin();  // Ensure ncurses exits before error
  //    return NULL;
  //  }
  //}

  //// --- FIX: Destroy login window before dashboard ---
  //delwin(logo_window);  // Delete login window
  //refresh();            // Clear the screen

  // Initialize dashboard panels and start render loop
  init_renderer();
  render_loop();

  // Cleanup
  clear_renderer();
  endwin();
  return NULL;
}
