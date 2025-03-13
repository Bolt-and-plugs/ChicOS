#include "schedduler.h"
#include "../process/process.h"

#define MAX_TICKETS 100

extern process process_list[10];

i32 get_winner() {
  // lottery
  int counter = 0;
  srand(time(NULL));
  int winner = rand() % MAX_TICKETS;
  printf("winner ticket %d\n", winner);

  process winner_p;
  for (int i = 0; i < 10; i++) {
    winner_p = process_list[i];
    counter += winner_p.tickets;
    if (!process_list[i].pid) {
      winner_p = process_list[i - 1];
      break;
    }
    if (counter > winner) {
      break;
    }
  }
  return winner_p.pid;
}
