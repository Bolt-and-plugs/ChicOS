#include "render.h"
#include "../../chicos.h"
#include "../memory/mem.h"
#include "../user/user.h"
#include <ctype.h>
#include <locale.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>

extern App app;
volatile sig_atomic_t resized = 0;

void handle_resize(int sig) { resized = 1; }

// Initialize curses modes
void bootstrap_ui() {
  sem_init(&app.rdr.renderer_s, 0, 1);
  app.rdr.active = true;
  app.rdr.output_buff = c_alloc(4096);
  strcpy(app.rdr.output_buff, "init");
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  curs_set(0);
  refresh();
}

// Create a boxed window
WINDOW *create_newwin(int h, int w, int y, int x) {
  WINDOW *win = newwin(h, w, y, x);
  box(win, 0, 0);
  wrefresh(win);
  return win;
}

void clear_renderer() {
  delwin(app.rdr.status_win);
  delwin(app.rdr.left_panel);
  delwin(app.rdr.right_top);
  delwin(app.rdr.right_bottom);
  if (!app.debug) {
    WINDOW *goodbye = create_newwin(LINES, COLS, 0, 0);
    mvwprintw(goodbye, LINES / 2, (COLS - 10) / 2, "ATÉ MAIS!");
    wrefresh(goodbye);
    napms(2000);
    delwin(goodbye);
  }
  endwin();
  c_dealloc(app.rdr.output_buff);
  app.rdr.active = false;
}

void status_bar() {
  WINDOW *s = app.rdr.status_win;
  werase(s);
  mvwprintw(s, 0, 0, "ChicOS(S) | %sPress CTRL+C to quit",
            app.debug ? "Debug Mode | " : "");
  wrefresh(s);
}

void render_left_panel() {
  werase(app.rdr.left_panel);
  box(app.rdr.left_panel, 0, 0);
  mvwprintw(app.rdr.left_panel, 1, 1, "CPU - quantum time: %ld",
            app.cpu.quantum_time);

  if (strcmp(app.rdr.output_buff, "init") != 0) {
    mvwprintw(app.rdr.left_panel, 2, 1, app.rdr.output_buff);
    strcpy(app.rdr.output_buff, "init");
  }
  int i = 0;
  // app.pcb.last=1;
  while (i <= app.pcb.last) {
    if (!app.pcb.process_stack[0].address_space) {
      mvwprintw(app.rdr.left_panel, i + 2, 1, "%u  last: %u", i, app.pcb.last);
    } else {
      mvwprintw(app.rdr.left_panel, i + 2, 1,
                "\tProcess: %s\tid: %d\tSTATUS= % d ",
                app.pcb.process_stack[i].name, app.pcb.process_stack[i].pid,
                app.pcb.process_stack[i].status);
    }
    i++;
  }

  wrefresh(app.rdr.left_panel);
}

void render_right_top_panel() {
  WINDOW *p = app.rdr.right_top;
  werase(p);
  box(p, 0, 0);
  int y, x;
  getmaxyx(p, y, x);
  float used = retrieve_used_mem_percentage();
  int barw = x - 4;
  int filled = (int)(used * barw / 100.0);
  if (filled > barw)
    filled = barw;
  char bar[barw + 1];
  memset(bar, '=', filled);
  memset(bar + filled, '-', barw - filled);
  bar[barw] = '\0';
  mvwprintw(p, 1, 1, "Memory Usage:");
  mvwprintw(p, 2, 1, "[%s]", bar);
  wattron(p, used > 80 ? COLOR_PAIR(1) : COLOR_PAIR(2));
  mvwprintw(p, 2, 2, "%.*s", filled, bar);
  wattroff(p, COLOR_PAIR(1) | COLOR_PAIR(2));
  mvwprintw(p, 4, 1, "Used: %2.2f%%", used);
  mvwprintw(p, 5, 1, "Free: %2.2f%%", retrieve_free_mem_percentage());
  wrefresh(p);
}

void read_path(WINDOW *p) {
  nodelay(p, TRUE); // nodelay TRUE para não travar
  keypad(p, TRUE);

  char path_buffer[MAX_ADDRESS_SIZE];
  path_buffer[0] = '\0'; // inicializa vazio

  int idx = 0, maker;

  while (1) {
    maker = wgetch(p);

    if (maker != ERR) { // só processa se houve tecla
      if ((maker == KEY_BACKSPACE || maker == 127 || maker == 8) && idx > 0) {
        idx--;
        path_buffer[idx] = '\0';
      } else if (idx < sizeof(path_buffer) - 1 && maker >= 32 && maker <= 126) {
        path_buffer[idx++] = maker;
        path_buffer[idx] = '\0';
      } else if (maker == '\n') {
        break; // ENTER finaliza
      }
    }

    // Atualiza a janela sempre
    mvwprintw(p, 3, 1, "Path: %s", path_buffer); // mostra string
    wrefresh(p);
  }

  wrefresh(p);
  // mvwgetstr(p, 3, 1, path_buffer);
  nodelay(p, TRUE);

  if (open_file(path_buffer)) {
    mvwprintw(p, 4, 1, "File openned");
    mvwprintw(p, 5, 1, "Last file openned path: %s", path_buffer);
  } else {
    mvwprintw(p, 4, 1, "Error on file open");
  }
}

void render_right_bottom_panel() {
  WINDOW *p = app.rdr.right_bottom;
  box(p, 0, 0);
  mvwprintw(p, 1, 1, "Press 'p' and enter a path of a file");
  char char_to_stop = wgetch(p);
  if (char_to_stop == 'p' || char_to_stop == 'P')
    read_path(p);
  wrefresh(p);
}

// Initialize renderer windows
void init_renderer() {
  app.rdr.status_win = create_newwin(1, COLS, 0, 0);
  app.rdr.left_panel = create_newwin(LINES - 1, COLS / 2, 1, 0);
  app.rdr.right_top = create_newwin((LINES - 1) / 2, COLS / 2, 1, COLS / 2);
  app.rdr.right_bottom =
      create_newwin((LINES - 1) / 2, COLS / 2, 1 + (LINES - 1) / 2, COLS / 2);
}

// Main dashboard loop
void render_loop() {
  char path[MAX_ADDRESS_SIZE];
  signal(SIGWINCH, handle_resize);
  nodelay(stdscr, TRUE);
  while (!app.loop_stop) {
    if (resized) {
      resized = 0;
      endwin();
      refresh();
      bootstrap_ui();
      clear_renderer();
      init_renderer();
    }
    nodelay(app.rdr.right_bottom, TRUE);
    wbkgd(stdscr, COLOR_PAIR(1));
    status_bar();
    render_left_panel();
    render_right_top_panel();
    render_right_bottom_panel();
    napms(100);
  }
}

// Get credentials via a simple form
user get_credentials(WINDOW *win) {
  user login = {0};
  int y, x;
  getmaxyx(win, y, x);
  int w = 40, sx = (x - w) / 2;
  werase(win);
  box(win, 0, 0);
  mvwprintw(win, y / 2 - 2, sx, "Username:");
  WINDOW *u = derwin(win, 3, w - 10, y / 2 - 1, sx);
  box(u, 0, 0);
  wrefresh(u);
  mvwprintw(win, y / 2 + 2, sx, "Password:");
  WINDOW *p = derwin(win, 3, w - 10, y / 2 + 3, sx);
  box(p, 0, 0);
  wrefresh(p);
  wrefresh(win);
  // input
  echo();
  curs_set(1);
  mvwgetnstr(u, 1, 1, login.username, sizeof(login.username) - 1);
  noecho();
  keypad(p, TRUE);
  int ch, pos = 0;
  wmove(p, 1, 1);
  wrefresh(p);
  while ((ch = wgetch(p)) != '\n') {
    if ((ch == KEY_BACKSPACE || ch == 127 || ch == 8) && pos > 0) {
      pos--;
      mvwaddch(p, 1, pos + 1, ' ');
      wmove(p, 1, pos + 1);
    } else if (isprint(ch) && pos < sizeof(login.password) - 1) {
      login.password[pos++] = ch;
      mvwaddch(p, 1, pos, '*');
    }
    wrefresh(p);
  }
  login.password[pos] = '\0';
  curs_set(0);
  delwin(u);
  delwin(p);
  return login;
}

user *login_flow() {
  WINDOW *w = create_newwin(LINES - 1, COLS - 1, 0, 0);
  user login = {0};
  user *usr = NULL;
  char addr[MAX_ADDRESS_SIZE];
  bool logged_in = false;

  while (!logged_in) {
    memset(&login, 0, sizeof(login));
    werase(w);
    box(w, 0, 0);
    wrefresh(w);

    // Get credentials
    login = get_credentials(w);
    retrieve_addr(&login, addr, sizeof(addr));

    bool file_exists = (access(addr, F_OK) == 0);
    usr = read_login_data(&login);

    if (!file_exists) {
      mvwprintw(w, 2, 2, "User does not exist. Creating one...");
      wrefresh(w);
      write_login_data(&login);
      usr = read_login_data(&login);
      mvwprintw(w, 3, 2, usr->username, usr->password);
      wrefresh(w);

      if (usr) {
        logged_in = true;
        mvwprintw(w, 3, 2, "Welcome, %s!", usr->username);
        wrefresh(w);
        napms(2000);
      } else {
        mvwprintw(w, 3, 2, "Error creating user!");
        wrefresh(w);
        napms(2000);
      }
    } else if (usr != NULL) {
      // Existing user and credentials matched
      app.user = usr;
      logged_in = true;
    } else {
      // Wrong password
      mvwprintw(w, 2, 2, "Wrong password! Try again");
      wrefresh(w);
      napms(1000);
    }
  }

  delwin(w);
  clear();
  refresh();
  return usr;
}

void render_log(const char *statement) {
  semaphoreP(&app.rdr.renderer_s);
  napms(100);
  if (app.rdr.output_buff)
    strcpy(app.rdr.output_buff, statement);
  semaphoreV(&app.rdr.renderer_s);
}

void *init_render(void *arg) {
  bootstrap_ui();
  // app.user = login_flow();
  // if (!app.user) return NULL;
  init_renderer();
  render_loop();
  clear_renderer();

  return NULL;
}
