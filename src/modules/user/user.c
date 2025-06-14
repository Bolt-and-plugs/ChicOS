#include "user.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../memory/mem.h"
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define LOG_FILE "auth.log"

extern App app;

const char *current_timestamp() {
  static char buffer[20];
  time_t now = time(NULL);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
  return buffer;
}

void log_event(const char *event, const char *username) {
  FILE *log = fopen(LOG_FILE, "a");
  if (log) {
    fprintf(log, "[%s] %-20s %s\n", current_timestamp(), event, username);
    fclose(log);
  }
}

void retrieve_addr(const user *u, char *dest, size_t addr_sz) {
  mkdir("resources/data", 0755);
  snprintf(dest, addr_sz, "resources/data/%s.us", u->username);
}

void trim_whitespace(char *str) {
  if (!str)
    return;

  char *end = str + strlen(str) - 1;
  while (end >= str && isspace((unsigned char)*end))
    end--;
  *(end + 1) = '\0';

  char *start = str;
  while (isspace((unsigned char)*start))
    start++;
  memmove(str, start, end - start + 2);
}

char *encrypt(const char *password) {
  // TODO: Implement real encryption
  return (char *)password;
}

// Placeholder decryption
char *decrypt(const char *password) {
  // TODO: Implement real decryption
  return (char *)password;
}

void write_login_data(const user *u) {
  char addr[MAX_ADDRESS_SIZE];
  retrieve_addr(u, addr, sizeof(addr));

  FILE *fp = fopen(addr, "w");
  if (!fp) {
    log_event("FILE_CREATE_FAIL", u->username);
    c_crit_error(DISK_FILE_WRITE_ERROR, "Failed to create %s", addr);
    return;
  }

  fprintf(fp, "USERNAME: %s\nPASSWORD: %s\n", u->username,
          encrypt(u->password));
  fclose(fp);

  log_event("USER_CREATED", u->username);
}

user *read_login_data(const user *u) {
  char addr[MAX_ADDRESS_SIZE];
  retrieve_addr(u, addr, sizeof(addr));

  FILE *fp = fopen(addr, "r");
  if (!fp) {
    log_event("FILE_NOT_FOUND", u->username);
    return NULL;
  }

  user *local = c_alloc(sizeof(user));
  char line[MAX_LINE_LENGTH];
  int fields_found = 0;

  while (fgets(line, sizeof(line), fp)) {
    if (strncmp(line, "USERNAME: ", 10) == 0) {
      strncpy(local->username, line + 10, MAX_LINE_LENGTH);
      trim_whitespace(local->username);
      fields_found++;
    } else if (strncmp(line, "PASSWORD: ", 10) == 0) {
      strncpy(local->password, line + 10, MAX_LINE_LENGTH);
      trim_whitespace(local->password);
      fields_found++;
    }
  }
  fclose(fp);

  if (fields_found != 2) {
    log_event("INVALID_FILE_FORMAT", u->username);
    c_dealloc(local);
    return NULL;
  }

  log_event("FILE_READ_OK", u->username);

  if (app.debug) {
    FILE *log = fopen(LOG_FILE, "a");
    if (log) {
      fprintf(log, "[%s] DEBUG Read values - username: '%s' password: '%s'\n",
              current_timestamp(), local->username, local->password);
      fclose(log);
    }
  }

  if (strcmp(local->username, u->username) == 0 &&
      strcmp(decrypt(local->password), u->password) == 0) {
    local->logged = true;
    log_event("LOGIN_SUCCESS", u->username);
    return local;
  }

  log_event("LOGIN_FAIL", u->username);
  c_dealloc(local);
  return NULL;
}
