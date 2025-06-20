#include "render.h"
#include "../../chicos.h"
#include "../io/disk.h"
#include "../io/printer.h"
#include "../memory/mem.h"
#include "../user/user.h"
#include <ctype.h>
#include <locale.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>

extern App app;
volatile sig_atomic_t resized = 0;

void handle_resize(int sig) {
  if (sig) {
    resized = 1;
  }
}

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
  delwin(app.rdr.left_bottom);
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
            app.debug ? "Debug Mode | " : app.user->username);
  wrefresh(s);
}

void render_left_panel() {
  WINDOW *panel = app.rdr.left_panel;

  werase(panel);
  box(panel, 0, 0);

  mvwprintw(panel, 0, 1, " CPU | Quantum Time: %ld ", app.cpu.quantum_time);

  const int start_process_y = 2;
  const int col_name_x = 2;
  const int col_pid_x = 20;
  const int col_status_x = 28;
  const int col_time_x = 37;
  const int col_rw_count = 54;

  // print header in a table format (ONLY ONCE)
  wattron(panel, A_BOLD | A_UNDERLINE);
  mvwprintw(panel, start_process_y, col_name_x, "Process Name");
  mvwprintw(panel, start_process_y, col_pid_x, "PID");
  mvwprintw(panel, start_process_y, col_status_x, "Status");
  mvwprintw(panel, start_process_y, col_time_x, "CPU Time Left");
  mvwprintw(panel, start_process_y, col_rw_count, "R/W Count");
  wattroff(panel, A_BOLD | A_UNDERLINE);

  int current_row = start_process_y + 1;
  for (int i = 0; i < app.pcb.last; i++) {
    char status[10];
    switch (app.pcb.process_stack[i].status) {
    case RUNNING:
      strcpy(status, "RUNNING");
      break;
    case BLOCKED:
      strcpy(status, "BLOCKED");
      break;
    case WAITING:
      strcpy(status, "WAITING");
      break;
    case NEW:
      strcpy(status, "NEW");
      break;
    case READY:
      strcpy(status, "READY");
      break;
    case KILL:
      strcpy(status, "KILLING");
      break;
    default:
      strcpy(status, "IDLE");
      break;
    }

    mvwprintw(panel, current_row, col_name_x, "%-15.15s",
              app.pcb.process_stack[i].name);
    mvwprintw(panel, current_row, col_pid_x, "%-5d",
              app.pcb.process_stack[i].pid);
    mvwprintw(panel, current_row, col_status_x, "%-10s", status);
    mvwprintw(panel, current_row, col_time_x, "%-5u",
              app.pcb.process_stack[i].time_to_run);
    mvwprintw(panel, current_row, col_rw_count, "%-3u",
              app.pcb.process_stack[i].fb->h->rw_count);

    current_row++;
  }
  if (app.pcb.last == 0) {
    mvwprintw(panel, current_row, col_name_x, "------------");
    mvwprintw(panel, current_row, col_pid_x, "-----");
    mvwprintw(panel, current_row, col_status_x, "------");
    mvwprintw(panel, current_row, col_time_x, "-------------");
    mvwprintw(panel, current_row, col_rw_count, "---------");
  }

  if (app.pcb.last == MAX_PCB - 1)
    mvwprintw(panel, start_process_y + MAX_PCB, 2,
              "PCB is Full. No new process will be created");

  const int start_semaphore_y = MAX_PCB + 4;

  wattron(panel, A_BOLD | A_UNDERLINE);
  mvwprintw(panel, start_semaphore_y, col_name_x, "Semaphore Name");
  mvwprintw(panel, start_semaphore_y, col_pid_x, "SID");
  mvwprintw(panel, start_semaphore_y, col_status_x, "Value");
  mvwprintw(panel, start_semaphore_y, col_time_x, "Waiters");
  wattroff(panel, A_BOLD | A_UNDERLINE);

  current_row = start_semaphore_y + 1;
  int j;
  for (int i = 0; i < app.semaphores->last; i++) {
    mvwprintw(panel, current_row, col_name_x, "%c", app.semaphores->l[i].name);
    mvwprintw(panel, current_row, col_pid_x, "%-5d", app.semaphores->l[i].id);
    mvwprintw(panel, current_row, col_status_x, "%-2d",
              app.semaphores->l[i].value);
    j = col_time_x;
    for (int k = app.semaphores->l[i].head; k != app.semaphores->l[i].tail;
         k = (k + 1) % DEFAULT_WAITERS_NUM) {
      if (j < COLS / 2 - 1)
        mvwprintw(panel, current_row, j, "%d ",
                  app.semaphores->l[i].waiters[k]);
      j += 2;
    }
    current_row++;
  }

  if (app.semaphores->last == 0) {
    mvwprintw(panel, current_row, col_name_x, "-");
    mvwprintw(panel, current_row, col_pid_x, "-----");
    mvwprintw(panel, current_row, col_status_x, "--");
    mvwprintw(panel, current_row, col_time_x, "---");
  }

  wrefresh(panel);
}

void render_right_top_panel() {
  WINDOW *p = app.rdr.right_top;
  werase(p);
  box(p, 0, 0);
  int x;
  x = getmaxx(p);
  float used = retrieve_used_mem_percentage();
  int barw = x - 4;
  int filled = (int)(used * barw / 100.0);
  if (filled > barw)
    filled = barw;
  char bar[barw + 1];
  mvwprintw(p, 0, 1, " Memory Status: ");
  memset(bar, '=', filled);
  memset(bar + filled, '-', barw - filled);
  bar[barw] = '\0';
  mvwprintw(p, 2, 1, "RAM Usage:");
  mvwprintw(p, 3, 1, "[%s]", bar);
  wattron(p, used > 80 ? COLOR_PAIR(1) : COLOR_PAIR(2));
  mvwprintw(p, 3, 2, "%.*s", filled, bar);
  wattroff(p, COLOR_PAIR(1) | COLOR_PAIR(2));
  mvwprintw(p, 5, 1, " Used: %2.2f%% \\ Free: %2.2f%%", used,
            retrieve_free_mem_percentage());
  wrefresh(p);
}

int read_path(WINDOW *p) {
  char path_buffer[MAX_ADDRESS_SIZE] = {0};
  int max_x;
  max_x = getmaxx(p);

  keypad(p, TRUE);
  noecho();

  wmove(p, 4, 1);
  wclrtoeol(p);
  wmove(p, 5, 1);
  wclrtoeol(p);

  mvwprintw(p, 3, 1, "Path: ");
  box(p, 0, 0);
  wrefresh(p);

  u32 idx = 0;
  int ch;

  while (1) {
    ch = wgetch(p);

    if (ch == ERR)
      continue;

    if (ch == '\n') {
      break;
    } else if (ch == 27) {
      return 0;
    } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
      if (idx > 0) {
        idx--;
        path_buffer[idx] = '\0';
      }
    } else if (idx < sizeof(path_buffer) - 1 && ch >= 32 && ch <= 126) {
      path_buffer[idx++] = ch;
      path_buffer[idx] = '\0';
    }

    mvwprintw(p, 3, 7, "%-*s", max_x - 8, path_buffer);
    wmove(p, 3, 7 + idx);
    wrefresh(p);
  }

  if (valid_path(path_buffer)) {
    interrupt_control(process_create, "%s", path_buffer);
    mvwprintw(p, 5, 1, "Last file opened: %s", path_buffer);
    wclrtoeol(p);
  } else {
    mvwprintw(p, 4, 1, "Error: Invalid or non-existent file path.");
    wclrtoeol(p);
  }

  wrefresh(p);
  return 1;
}

void print_event(WINDOW *p) {
  print_list *buff = get_print_time(); // This function is now thread-safe.
  if (app.printer.active && buff != NULL) {
    mvwprintw(p, 2, 2, "Process %u Printing for: %u", buff->pid, buff->time);
  } else {

    mvwprintw(p, 2, 2, "Printer is empty          ");
  }
}
void render_left_bottom_panel() {
  WINDOW *panel = app.rdr.left_bottom;
  werase(panel);
  box(panel, 0, 0);

  mvwprintw(panel, 0, 1, " Printer: ");

  // nodelay(panel, FALSE);
  print_event(panel);

  wrefresh(panel);
}

void render_right_bottom_panel() {
  WINDOW *p = app.rdr.right_bottom;
  box(p, 0, 0);

  mvwprintw(p, 0, 1, " User Input: ");
  mvwprintw(p, 1, 2, "Press 'p' and enter a path of a file");

  char char_to_stop = wgetch(p);
  echo();

  int cancel = 0;
  if (char_to_stop == 'p' || char_to_stop == 'P' || cancel)
    cancel = read_path(p);
  noecho();

  wrefresh(p);
}

void draw_disk_status(WINDOW *p, int current_track) {
  int y, x;
  getmaxyx(p, y, x);

  int barw = x - 5;
  if (barw < 2)
    return;

  int track_pos = (int)(((float)current_track / (float)(TOTAL_TRACKS)) * barw);

  if (track_pos >= barw)
    track_pos = barw - 1;
  if (track_pos < 0)
    track_pos = 0;

  char bar_bg[barw + 1];
  memset(bar_bg, '-', barw);
  bar_bg[barw] = '\0';
  mvwprintw(p, y - 3, 1, " Disk Head Position Tracker %05d/%05d", current_track,
            TOTAL_TRACKS);
  mvwprintw(p, y - 2, 2, "[%s]", bar_bg);
  wattron(p, COLOR_PAIR(2));
  mvwprintw(p, y - 2, 3 + track_pos, "=");
  wattroff(p, COLOR_PAIR(2));
}

void render_right_mid_panel() {
  WINDOW *p = app.rdr.right_mid;
  mvwprintw(p, 0, 1, " Disk Status | %d to be done", app.disk.qr.len);

  const int start_disk_y = 1;
  const int col_id_x = 2;
  const int col_track_x = 10;
  const int col_time_x = 20;

  wattron(p, A_BOLD | A_UNDERLINE);
  mvwprintw(p, start_disk_y, col_id_x, "ID");
  mvwprintw(p, start_disk_y, col_track_x, "Track");
  mvwprintw(p, start_disk_y, col_time_x, "Time to Run");
  wattroff(p, A_BOLD | A_UNDERLINE);

  int current_row = start_disk_y + 1;
  for (int i = 0; i < app.disk.qr.len; i++) {
    mvwprintw(p, current_row, col_id_x, "%-3d", app.disk.qr.queue[i].id);
    mvwprintw(p, current_row, col_track_x, "%05d", app.disk.qr.queue[i].track);
    mvwprintw(p, current_row, col_time_x, "%-5d",
              app.disk.qr.queue[i].time_to_run);
    current_row++;
  }

  for (int i = app.disk.qr.len; i < MAX_PCB; i++) {
    mvwprintw(p, current_row, col_id_x, "   ");
    mvwprintw(p, current_row, col_track_x, "     ");
    mvwprintw(p, current_row, col_time_x, "     ");
    current_row++;
  }

  if (app.disk.qr.len == 0) {
    mvwprintw(p, 2, col_id_x, "---");
    mvwprintw(p, 2, col_track_x, "-----");
    mvwprintw(p, 2, col_time_x, "-----");
  }

  draw_disk_status(p, app.disk.current_track);

  wrefresh(p);
}

void init_renderer() {
  app.rdr.status_win = create_newwin(1, COLS, 0, 0);
  int left_panel_height = LINES - 11;
  int left_bottom_height = 10;
  app.rdr.left_panel = create_newwin(left_panel_height, COLS / 2, 1, 0);
  app.rdr.left_bottom =
      create_newwin(left_bottom_height, COLS / 2, 1 + left_panel_height, 0);

  const int RIGHT_BOTTOM_FIXED_HEIGHT = 6;

  int start_y = 1;
  int start_x = COLS / 2;
  int width = COLS / 2;
  int available_height = LINES - 1;
  int remaining_height = available_height - RIGHT_BOTTOM_FIXED_HEIGHT;
  int height_top = remaining_height / 2;
  int height_mid = remaining_height - height_top;

  app.rdr.right_top = create_newwin(height_top, width, start_y, start_x);
  app.rdr.right_mid =
      create_newwin(height_mid, width, start_y + height_top, start_x);
  app.rdr.right_bottom =
      create_newwin(RIGHT_BOTTOM_FIXED_HEIGHT, width,
                    start_y + height_top + height_mid, start_x);
}

void render_loop() {
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
    render_right_mid_panel();
    render_right_bottom_panel();
    render_left_bottom_panel();
    napms(100);
  }
}

user get_credentials(WINDOW *win) {
  user login = {0};
  int y, x;
  getmaxyx(win, y, x);
  int w = 40, sx = (x - w) / 2;
  werase(win);

  print_logo(win);
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
  echo();
  curs_set(1);
  mvwgetnstr(u, 1, 1, login.username, sizeof(login.username) - 1);
  noecho();
  keypad(p, TRUE);
  int ch = 0;
  u32 pos = 0;
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

    login = get_credentials(w);
    retrieve_addr(&login, addr, sizeof(addr));

    bool file_exists = (access(addr, F_OK) == 0);
    usr = read_login_data(&login);

    if (!file_exists) {
      mvwprintw(w, LINES - 2, (COLS / 2) - 11,
                "User does not exist. Creating one...");
      wrefresh(w);
      write_login_data(&login);
      usr = read_login_data(&login);
      // mvwprintw(w, LINES - 1, (COLS / 2) - (18 + strlen(usr->password)),
      //           "User '%s' created with password '%s'", usr->username,
      //           app.debug ? usr->password : "****");
      wrefresh(w);

      if (usr) {
        logged_in = true;
      } else {
        wattron(w, COLOR_PAIR(1));
        mvwprintw(w, (LINES / 2) - 1, 2, "Error creating user!");
        wattroff(w, COLOR_PAIR(1));
        wrefresh(w);
        napms(2000);
      }
    } else if (usr != NULL) {
      app.user = usr;
      logged_in = true;
    } else {
      wattron(w, COLOR_PAIR(1));
      mvwprintw(w, 2, 2, "Wrong password! Try again");
      wattroff(w, COLOR_PAIR(1));
      wrefresh(w);
      napms(1000);
    }
  }

  delwin(w);
  clear();
  refresh();
  return usr;
}

void render_log(char *statement) {
  sem_wait(&app.rdr.renderer_s);
  napms(100);
  if (app.rdr.output_buff)
    strcpy(app.rdr.output_buff, statement);
  sem_post(&app.rdr.renderer_s);
}

void draw_dots(WINDOW *win, int repetitions) {
  int start_y = LINES / 2;
  int start_x = (COLS - 8) / 2 + 4;
  int delay_ms = 100;
  mvwprintw(win, start_y + 1, start_x - 8, "[            ]");

  wrefresh(win);
  for (int i = 0; i < repetitions; i++) {
    for (int k = 0; k < 12; k++) {
      mvwprintw(win, start_y + 1, start_x - 7 + k, ".");
      wrefresh(win);
      napms(delay_ms);
    }
    napms(300);
    for (int k = 0; k < 12; k++) {
      mvwprintw(win, start_y + 1, start_x - 7 + k, " ");
      wrefresh(win);
      napms(delay_ms / 2);
    }
    napms(300);
  }
}

void print_logo(WINDOW *p) {
  mvwprintw(
      p, 2, (COLS - 98) / 2,
      "________/\\\\\\\\\\\\\\\\\\__/\\\\\\___________________________________/"
      "\\\\\\\\\\__________/\\\\\\\\\\\\\\\\\\\\\\___        ");
  mvwprintw(p, 3, (COLS - 98) / 2,
            " _____/\\\\\\////////__\\/\\\\\\_________________________________/"
            "\\\\\\///\\\\\\______/\\\\\\/////////\\\\\\_       ");
  mvwprintw(p, 4, (COLS - 98) / 2,
            "  "
            "___/\\\\\\/___________\\/\\\\\\__________/\\\\\\_________________/"
            "\\\\\\/__\\///\\\\\\___\\//\\\\\\______\\///__      ");
  mvwprintw(
      p, 5, (COLS - 98) / 2,
      "   "
      "__/\\\\\\_____________\\/\\\\\\_________\\///______/\\\\\\\\\\\\\\\\__/"
      "\\\\\\______\\//\\\\\\___\\////\\\\\\_________     ");
  mvwprintw(
      p, 6, (COLS - 98) / 2,
      "    "
      "_\\/\\\\\\_____________\\/\\\\\\\\\\\\\\\\\\\\___/\\\\\\___/\\\\\\//////"
      "__\\/\\\\\\_______\\/\\\\\\______\\////\\\\\\______    ");
  mvwprintw(
      p, 7, (COLS - 98) / 2,
      "     "
      "_\\//\\\\\\____________\\/\\\\\\/////\\\\\\_\\/\\\\\\__/"
      "\\\\\\_________\\//\\\\\\______/\\\\\\__________\\////\\\\\\___   ");
  mvwprintw(
      p, 8, (COLS - 98) / 2,
      "      "
      "__\\///\\\\\\__________\\/\\\\\\___\\/\\\\\\_\\/\\\\\\_\\//"
      "\\\\\\_________\\///\\\\\\__/\\\\\\_____/\\\\\\______\\//\\\\\\__  ");
  mvwprintw(p, 9, (COLS - 98) / 2,
            "       "
            "____\\////\\\\\\\\\\\\\\\\\\_\\/\\\\\\___\\/\\\\\\_\\/\\\\\\__\\//"
            "/\\\\\\\\\\\\\\\\____\\///\\\\\\\\\\/_____\\///"
            "\\\\\\\\\\\\\\\\\\\\\\/___ ");
  mvwprintw(p, 10, (COLS - 98) / 2,
            "        "
            "_______\\/////////__\\///____\\///__\\///_____\\////////_______\\/"
            "////_________\\///////////_____");
}

void welcome_screen() {
  WINDOW *welcome = create_newwin(LINES, COLS, 0, 0);
  int names_y = LINES - 5;
  const char *createdBy = "Created By", *name1 = "CEN-s",
             *name2 = "Felipe-gsilva", *name3 = "marioluci0",
             *name4 = "RenanSpim";

  print_logo(welcome);

  mvwprintw(welcome, LINES / 2,
            ((COLS - 11) / 2) - strlen(app.user->username) / 2, "Bem vindo %s!",
            app.user->username);

  mvwprintw(welcome, names_y, (COLS - strlen(createdBy)) / 2, "%s", createdBy);

  int name_line_y = names_y + 2;
  int quadrant_width = COLS / 4;

  mvwprintw(welcome, name_line_y,
            (0 * quadrant_width) + (quadrant_width - strlen(name1)) / 2, "%s",
            name1);
  mvwprintw(welcome, name_line_y,
            (1 * quadrant_width) + (quadrant_width - strlen(name2)) / 2, "%s",
            name2);
  mvwprintw(welcome, name_line_y,
            (2 * quadrant_width) + (quadrant_width - strlen(name3)) / 2, "%s",
            name3);
  mvwprintw(welcome, name_line_y,
            (3 * quadrant_width) + (quadrant_width - strlen(name4)) / 2, "%s",
            name4);

  wrefresh(welcome);

  draw_dots(welcome, 3);

  delwin(welcome);
}

void *init_render(void *arg) {
  if (app.debug)
    c_info("Initializing renderer %s", arg);
  bootstrap_ui();
  if (!app.debug) {
    app.user = login_flow();
    if (!app.user)
      return NULL;
    welcome_screen();
  }
  app.user = login_flow(); 
  if (!app.user) return NULL;
  init_renderer();
  render_loop();
  clear_renderer();

  return NULL;
}
