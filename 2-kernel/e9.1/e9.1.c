#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 5
#define MAX_THINKING_TIME 3
#define MAX_EATING_TIME 2
#define SIMULATION_TIME 30

pthread_mutex_t forks[N];
pthread_t philosophers[N];
volatile int running = 1;

void *philosopher(void *arg) {
    int id = *((int *)arg);
    int left_fork = id;
    int right_fork = (id + 1) % N;

    while (running) {
        printf("Philosopher %d is thinking\n", id);
        sleep(rand() % MAX_THINKING_TIME + 1);

        printf("Philosopher %d is hungry and trying to get forks\n", id);

        // if (id == N - 1) { optional
        pthread_mutex_lock(&forks[right_fork]);
        printf("Philosopher %d picked up right fork %d\n", id, right_fork);

        pthread_mutex_lock(&forks[left_fork]);
        printf("Philosopher %d picked up left fork %d\n", id, left_fork);
        // } else { optional
        //     pthread_mutex_lock(&forks[left_fork]);
        //     printf("Philosopher %d picked up left fork %d\n", id, left_fork);
        //
        //     pthread_mutex_lock(&forks[right_fork]);
        //     printf("Philosopher %d picked up right fork %d\n", id, right_fork);
        // }

        printf("Philosopher %d is eating\n", id);
        sleep(rand() % MAX_EATING_TIME + 1);

        pthread_mutex_unlock(&forks[left_fork]);
        printf("Philosopher %d put down left fork %d\n", id, left_fork);

        pthread_mutex_unlock(&forks[right_fork]);
        printf("Philosopher %d put down right fork %d\n", id, right_fork);
    }

    printf("Philosopher %d is leaving the table\n", id);
    pthread_exit(NULL);
}

int main() {
    int ids[N];
    time_t end_time;

    srand(time(NULL));

    printf("Dining Philosophers Problem Simulation\n");
    printf("Number of philosophers: %d\n", N);
    printf("Simulation will run for %d seconds\n", SIMULATION_TIME);

    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    end_time = time(NULL) + SIMULATION_TIME;
    while (time(NULL) < end_time) {
        sleep(1);
    }

    running = 0;
    printf("Simulation ending...\n");

    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < N; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    printf("Simulation ended\n");
    return 0;
}
