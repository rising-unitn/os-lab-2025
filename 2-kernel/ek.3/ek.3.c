#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define EXPERIMENT_DUR 20

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty, full, mutex;

volatile int running = 1;

void *producer(void *arg) {
    while (running) {
        // Produce a random number
        int item = rand() % 100;

        // Wait for an empty slot
        sem_wait(&empty);

        // Acquire mutex for buffer access
        sem_wait(&mutex);

        // Add item to buffer
        buffer[in] = item;
        printf("Producer: Inserted %d at position %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;

        // Release mutex
        sem_post(&mutex);

        // Signal that a slot is now full
        sem_post(&full);

        // Sleep for a short time
        sleep(rand() % 5);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    while (running) {
        // Wait for a full slot
        sem_wait(&full);

        // Acquire mutex for buffer access
        sem_wait(&mutex);

        // Remove item from buffer
        int item = buffer[out];
        printf("Consumer: Removed %d from position %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        // Release mutex
        sem_post(&mutex);

        // Signal that a slot is now empty
        sem_post(&empty);

        // Sleep for a short time
        usleep(rand() % 8);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod, cons;
    pthread_attr_t prod_attr, cons_attr;
    time_t end_time;

    srand(time(NULL));

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    printf("Bounded Buffer Simulation\n");
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("Simulation will run for %d seconds\n", EXPERIMENT_DUR);

    pthread_attr_init(&prod_attr);
    pthread_attr_init(&cons_attr);
    pthread_create(&prod, &prod_attr, producer, NULL);
    pthread_create(&cons, &cons_attr, consumer, NULL);

    end_time = time(NULL) + EXPERIMENT_DUR;
    while (time(NULL) < end_time) {
        sleep(1);
    }

    running = 0;
    printf("Simulation ending...\n");

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    printf("Simulation ended\n");
    return 0;
}
