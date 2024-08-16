#include <signal.h>
#include <stdio.h>
#include <threads.h>
#include <time.h>

void my_handler(int sig) {
  if (sig == SIGINT) {
    printf("Haha not stopping\n");
  }
}

int main() {
  if (signal(SIGINT, my_handler) < 0) {
    perror("singal: ");
  }

  struct timespec duration = {.tv_sec = 1};

  do {
    puts("Cant stop me!!");
    thrd_sleep(&duration, NULL);
  } while (1);
}