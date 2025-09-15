#include <stdio.h>
#include <signal.h>
#include <unistd.h>

struct sigaction act;

void handler(int signum) {
  printf("Caught signal %d\n", signum);
}

int main() {
  act.sa_handler = handler;
  sigaction(SIGINT, &act, NULL);
  while(1) {
    sleep(1);
  }
  return 0;
}
