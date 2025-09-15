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
    // implement your code here
}

void *consumer(void *arg) {
    int id = *((int *)arg);
    // implement your code here
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

    // implement your code here:

    // create the pipe
    // create producer threads
    // create consumer threads

    end_time = time(NULL) + SIMULATION_TIME;
    while (time(NULL) < end_time) {
        sleep(1);
    }

    running = 0;
    printf("Simulation ending...\n");

    // implement your code here:

    // handle closing of the pipe (if necessary)
    // wait for all producer threads to finish
    // wait for all consumer threads to finish

    printf("Simulation ended\n");
    return 0;
}
