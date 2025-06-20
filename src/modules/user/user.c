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

void initialize_crypto() {
    if (sodium_init() < 0) {
        abort();
    }
}

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
    char *hashed_password = (char *)malloc(crypto_pwhash_STRBYTES);

    if (!hashed_password) {
        return NULL;
    }

    if (crypto_pwhash_str(hashed_password, password, strlen(password),
                          crypto_pwhash_OPSLIMIT_INTERACTIVE,
                          crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        free(hashed_password);
        c_crit_error(USER_HASH_ERROR, "Falha ao gerar o hash da senha");
        return NULL;
    }

    return hashed_password;
}

bool decrypt(const char *stored_hash, const char *provided_password) {
    if (crypto_pwhash_str_verify(stored_hash, provided_password, strlen(provided_password)) != 0) {
        return false;
    }
    return true;
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

  char *hashed_password = encrypt(u->password);
  if (hashed_password) {
    fprintf(fp, "USERNAME: %s\nPASSWORD: %s\n", u->username,
            hashed_password);
    free(hashed_password);
  }
  fflush(fp);
  fclose(fp);

  log_event("USER_CREATED", u->username);
}

user *read_login_data(const user *u) {
    char addr[MAX_ADDRESS_SIZE];
    retrieve_addr(u, addr, sizeof(addr));

    FILE *fp = fopen(addr, "r");
    if (!fp) {
        log_event("FILE_NOT_FOUND", u->username);

        // write_login_data(u);

        return NULL;
    }

    user *local = c_alloc(sizeof(user));
    if (!local) {
        fclose(fp);
        return NULL;
    }
    memset(local, 0, sizeof(user));

    char line[MAX_LINE_LENGTH];
    int fields_found = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "USERNAME: ", 10) == 0) {
            snprintf(local->username, sizeof(local->username), "%s", line + 10);
            trim_whitespace(local->username);
            fields_found++;
        } else if (strncmp(line, "PASSWORD: ", 10) == 0) {
            snprintf(local->password, sizeof(local->password), "%s", line + 10);
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

    if (strcmp(local->username, u->username) == 0 &&
        decrypt(local->password, u->password) == true) {
        local->logged = true;
        log_event("LOGIN_SUCCESS", u->username);
        return local;
    } else {
      log_event("ERRO_AQUI", local->password);
    }

    log_event("LOGIN_FAIL", u->username);
    log_event("LOGIN_FAIL", local->username);
    log_event("LOGIN_FAIL", u->password);
    log_event("LOGIN_FAIL", local->password);
    if (decrypt(local->password, u->password) == true) log_event("CRIPTOGRAFIA", local->password);
    
    c_dealloc(local);
    return NULL;
}