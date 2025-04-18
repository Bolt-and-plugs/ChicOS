#include "semaphore.h"
#include "../log/log.h"

void semaphoreP(sem_t *s) {
  if (!s) {
    c_error(SEMAPHORE_WAIT_ERROR, "Semaphore does not exist");
    return;
  }
  sem_wait(s);
}

void semaphoreV(sem_t *s) {
  if (!s) {
    c_error(SEMAPHORE_POST_ERROR, "Semaphore does not exist");
    return;
  }
  sem_post(s);
}
