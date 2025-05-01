#ifndef _SEMAPHORE
#define _SEMAPHORE

#include <semaphore.h>
void interrupControl(void);
void sysCall(void);

void semaphoreP(sem_t *s);
void semaphoreV(sem_t *s);

#endif
