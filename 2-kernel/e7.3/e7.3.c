#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int counter = 0;

void handle_sigusr1(int signum) {
    printf("Received SIGUSR1: Incrementing counter.\n");
    counter++;
}

void handle_sigusr2(int signum) {
    printf("Received SIGUSR2: Printing counter value: %d\n", counter);
    counter = 0;
}

void handle_sigterm(int signum) {
    printf("Received SIGTERM: Exiting gracefully. Final counter value: %d\n", counter);
    exit(EXIT_SUCCESS);
}

int main() {
    struct sigaction sa_usr1, sa_usr2, sa_term;
    printf("Signal catcher started. PID: %d\n", getpid());

    sa_usr1.sa_handler = handle_sigusr1;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
        perror("sigaction SIGUSR1 failed");
        exit(EXIT_FAILURE);
    }

    sa_usr2.sa_handler = handle_sigusr2;
    sigemptyset(&sa_usr2.sa_mask);
    sa_usr2.sa_flags = 0;
    if (sigaction(SIGUSR2, &sa_usr2, NULL) == -1) {
        perror("sigaction SIGUSR2 failed");
        exit(EXIT_FAILURE);
    }

    sa_term.sa_handler = handle_sigterm;
    sigemptyset(&sa_term.sa_mask);
    sa_term.sa_flags = 0;
    if (sigaction(SIGTERM, &sa_term, NULL) == -1) {
        perror("sigaction SIGTERM failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Working...\n");
        sleep(2);
    }

    return EXIT_SUCCESS;
}
