#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void create_process_tree(int p, int d, int current_layer) {
    if (current_layer > d) {
        return;
    }

    for (int i = 0; i < p; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // printf("Process created: PID = %d, Parent PID = %d, Layer = %d\n", getpid(), getppid(), current_layer);
            FILE *file;
            char filename[20];
            snprintf(filename, sizeof(filename), "%d.txt", getpid());
            file = fopen(filename, "w");
            if (file == NULL) {
                perror("File open failed");
                exit(EXIT_FAILURE);
            }
            fprintf(file, "PID: %d, Parent PID: %d\n", getpid(), getppid());
            fclose(file);

            create_process_tree(p, d, current_layer + 1);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < p; i++) {
        wait(NULL);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_processes (p)> <number_of_layers (d)>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int p = atoi(argv[1]);
    int d = atoi(argv[2]);

    if (p <= 0 || d <= 0 || d >= 10 || p <= d) {
        fprintf(stderr, "Invalid arguments: p > 0, d > 0, d < 10, p > d\n");
        exit(EXIT_FAILURE);
    }

    create_process_tree(p, d, 1);

    return 0;
}