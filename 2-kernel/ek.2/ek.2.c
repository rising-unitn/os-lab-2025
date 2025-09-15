#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int fd;

    fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        printf("Output from child\n");
        exit(EXIT_SUCCESS);
    } else {
        int saved_stdout = dup(STDOUT_FILENO);

        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        printf("Output from parent\n");
        fflush(stdout);

        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);

        close(fd);

        wait(NULL);

        printf("Check output.txt to verify only parent's output was redirected\n");

        return 0;
    }
}
