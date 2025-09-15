#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int pipe1[2], pipe2[2];
    pid_t pid1, pid2, pid3;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <search_term>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if ((pid1 = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);

        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[1]);

        execlp("ls", "ls", NULL);
        perror("execlp ls");
        exit(EXIT_FAILURE);
    }

    if ((pid2 = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        close(pipe1[1]);
        close(pipe2[0]);

        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe1[0]);
        close(pipe2[1]);

        execlp("grep", "grep", argv[1], NULL);
        perror("execlp grep");
        exit(EXIT_FAILURE);
    }

    if ((pid3 = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid3 == 0) {
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);

        dup2(pipe2[0], STDIN_FILENO);
        close(pipe2[0]);

        execlp("wc", "wc", "-l", NULL);
        perror("execlp wc");
        exit(EXIT_FAILURE);
    }

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    return 0;
}
