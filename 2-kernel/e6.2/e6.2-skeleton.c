#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 80

int main(void) {
    char *args[MAX_LINE / 2 + 1];
    char input[MAX_LINE];
    int should_run = 1;

    while (should_run) {
        printf("unitn-shell$ ");
        fflush(stdout);

        if (fgets(input, MAX_LINE, stdin) == NULL) {
            perror("fgets failed");
            continue;
        }

        input[strcspn(input, "\n")] = 0;

        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
            continue;
        }
    }

    return 0;
}