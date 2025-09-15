#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define N_PRODUCERS 3
#define M_CONSUMERS 2
#define SIMULATION_TIME 20
#define MAX_PRODUCTION_TIME 3
#define MAX_CONSUMPTION_TIME 4

int pipe_fd[2];
pthread_t producers[N_PRODUCERS];
pthread_t consumers[M_CONSUMERS];
volatile int running = 1;

void *producer(void *arg) {
    int id = *((int *)arg);

    while (running) {
        int number = rand() % 100;
        sleep(rand() % MAX_PRODUCTION_TIME + 1);

        write(pipe_fd[1], &number, sizeof(int));
        printf("Producer %d produced: %d\n", id, number);
    }

    printf("Producer %d exiting\n", id);
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int id = *((int *)arg);
    int number;
    ssize_t bytes_read;

    while (running) {
        sleep(rand() % MAX_CONSUMPTION_TIME + 1);

        bytes_read = read(pipe_fd[0], &number, sizeof(int));
        if (bytes_read == sizeof(int)) {
            printf("Consumer %d consumed: %d\n", id, number);
        } else if (bytes_read == 0) {
            printf("Consumer %d found pipe closed\n", id);
            break;
        } else {
            perror("read");
            break;
        }
    }

    printf("Consumer %d exiting\n", id);
    pthread_exit(NULL);
}

int main() {
    int producer_ids[N_PRODUCERS];
    int consumer_ids[M_CONSUMERS];
    time_t end_time;

    srand(time(NULL));

    printf("Producer-Consumer Problem Simulation\n");
    printf("Number of producers: %d\n", N_PRODUCERS);
    printf("Number of consumers: %d\n", M_CONSUMERS);
    printf("Simulation will run for %d seconds\n", SIMULATION_TIME);

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N_PRODUCERS; i++) {
        producer_ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    for (int i = 0; i < M_CONSUMERS; i++) {
        consumer_ids[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }

    end_time = time(NULL) + SIMULATION_TIME;
    while (time(NULL) < end_time) {
        sleep(1);
    }

    running = 0;
    printf("Simulation ending...\n");

    close(pipe_fd[1]);

    for (int i = 0; i < N_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < M_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    close(pipe_fd[0]);

    printf("Simulation ended\n");
    return 0;
}
