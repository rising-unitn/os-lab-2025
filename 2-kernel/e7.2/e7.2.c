#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t child1 = 0, child2 = 0;

void handle_sigusr2(int sig) {
    if (child1 == 0) {
    }
    if (child2 == 0) {
    }
    printf("Received SIGUSR2, handling...\n");
    sleep(3);
    kill(child1, SIGSTOP);
    printf("Child 1 suspended\n");
    sleep(3);
    kill(child1, SIGCONT);
    printf("Child 1 resumed\n");
    sleep(3);
    kill(child1, SIGKILL);
    kill(child2, SIGKILL);
    printf("Both children unalived\n");
    exit(0);
}

void create_child_processes() {
    child1 = fork();
    if (child1 < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (child1 == 0) {
        while (1) {
            printf("Child 1 process %d is alive\n", getpid());
            sleep(1);
        }
    }

    child2 = fork();
    if (child2 < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (child2 == 0) {
        while (1) {
            printf("Child 2 process %d is alive\n", getpid());
            sleep(1);
        }
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_sigusr2;

    if (sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("Error handling SIGUSR2");
        exit(EXIT_FAILURE);
    }

    create_child_processes();

    while (1) {
        printf("Parent process %d is alive\n", getpid());
        sleep(1);
    }

    return 0;
}