#include "user.h"
#include "../../defines.h"
#include "../io/file.h"
#include "../log/log.h"
#include "../memory/mem.h"

void retrieve_addr(const user *u, char *addr, size_t addr_sz) {
  if (snprintf(addr, addr_sz, "data/%s.us", u->username) >= (int)addr_sz) {
    addr[addr_sz - 1] = '\0';
  }
}

char *encrypt(char *password) { return password; }

void write_login_data(user u) {
  char addr[MAX_ADDRESS_SIZE];
  retrieve_addr(&u, addr, sizeof(addr));
  FILE *fp = fopen(addr, "w");
  if (!fp) {
    c_crit_error(DISK_FILE_WRITE_ERROR, "Could not open %s for writing", addr);
    return;
  }

  if (fprintf(fp, "user: %s\npswd: %s\n", u.username, encrypt(u.password)) <
      0) {
    fclose(fp);
    c_crit_error(DISK_FILE_WRITE_ERROR, "Could not write user");
  }
  fclose(fp);
}

char *decrypt(char *password) { return password; }

user *read_login_data(user u) {
  char addr[MAX_ADDRESS_SIZE];
  retrieve_addr(&u, addr, MAX_ADDRESS_SIZE);
  FILE *fp = fopen(addr, "r");
  if (!fp)
    return NULL;

  user *local_user = alloc(sizeof(user));
  if (fscanf(fp,
             "user: %31s\n"
             "pswd: %31s\n",
             local_user->username, local_user->password) != 2) {
    dealloc(local_user);
    fclose(fp);
    return NULL;
  }

  printf("user: %31s\n pswd: %31s\n", local_user->username,
         local_user->password);
  printf("user: %31s\n pswd: %31s\n", u.username, u.password);

  fclose(fp);

  if (strcmp(local_user->username, u.username) == 0 &&
      strcmp(decrypt(local_user->password), u.password) == 0) {
    return local_user;
  }

  dealloc(local_user);
  return NULL;
}
