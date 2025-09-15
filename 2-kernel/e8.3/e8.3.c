#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_THREADS 5
#define ITERATIONS 10

pthread_t threads[NUM_THREADS];
int thread_counter[NUM_THREADS] = {0};
volatile int running = 1;

void *thread_function(void *arg) {
    int thread_id = *((int *)arg);

    while (running) {
        thread_counter[thread_id]++;
        printf("Thread %d - count: %d\n", thread_id, thread_counter[thread_id]);
        sleep(1);
    }

    printf("Thread %d exiting. Final count: %d\n", thread_id, thread_counter[thread_id]);
    pthread_exit(NULL);
}

void signal_handler(int signum) {
    printf("\nReceived signal %d\n", signum);

    if (signum == SIGUSR1) {
        printf("Thread counters: ");
        for (int i = 0; i < NUM_THREADS; i++) {
            printf("%d ", thread_counter[i]);
        }
        printf("\n");
    } else if (signum == SIGUSR2) {
        int max_count = 0;
        int max_thread = 0;

        for (int i = 0; i < NUM_THREADS; i++) {
            if (thread_counter[i] > max_count) {
                max_count = thread_counter[i];
                max_thread = i;
            }
        }

        printf("Thread %d has the highest count: %d\n", max_thread, max_count);
    } else if (signum == SIGINT) {
        printf("Terminating all threads...\n");
        running = 0;

        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        printf("All threads terminated. Final counts:\n");
        for (int i = 0; i < NUM_THREADS; i++) {
            printf("Thread %d: %d\n", i, thread_counter[i]);
        }

        exit(0);
    }
}

int main() {
    int thread_ids[NUM_THREADS];
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;

    if (sigaction(SIGUSR1, &sa, NULL) != 0) {
        perror("Error setting up SIGUSR1 handler");
        return 1;
    }

    if (sigaction(SIGUSR2, &sa, NULL) != 0) {
        perror("Error setting up SIGUSR2 handler");
        return 1;
    }

    if (sigaction(SIGINT, &sa, NULL) != 0) {
        perror("Error setting up SIGINT handler");
        return 1;
    }

    printf("Starting %d threads. Process PID: %d\n", NUM_THREADS, getpid());
    printf("Available signals:\n");
    printf("- SIGUSR1 (kill -SIGUSR1 %d): Display all thread counters\n", getpid());
    printf("- SIGUSR2 (kill -SIGUSR2 %d): Display thread with highest counter\n", getpid());
    printf("- SIGINT (Ctrl+C or kill -SIGINT %d): Terminate program\n", getpid());

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    while (running) {
        sleep(1);
    }

    return 0;
}
