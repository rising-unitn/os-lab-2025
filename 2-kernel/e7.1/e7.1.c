#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_signal(int sig) {
    if (sig == SIGUSR1) {
        printf("Hello from SIGUSR1\n");
    } else if (sig == SIGUSR2) {
        printf("Hello from SIGUSR2\n");
    }
    // fflush(stdout);
}

int main() {
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    // sa.sa_flags = 0;
    // sigemptyset(&sa.sa_mask);

    // sigaction(SIGUSR1, &sa, NULL);
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Error handling SIGUSR1");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("Error handling SIGUSR2");
        exit(EXIT_FAILURE);
    }

    while (1) {
        pause();
    }

    return 0;
}