#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_READERS 3
#define NUM_WRITERS 2
#define EXPERIMENT_DUR 30

int shared_counter = 0;

pthread_mutex_t mutex;
pthread_cond_t readers_done, writers_done;
int reader_count = 0;
int writer_active = 0;

volatile int running = 1;

void *reader(void *arg) {
    int id = *((int *)arg);

    while (running) {
        // Entry section
        pthread_mutex_lock(&mutex);

        // Wait if there's a writer
        while (writer_active && running) {
            printf("Reader %d waiting for writers to finish\n", id);
            pthread_cond_wait(&writers_done, &mutex);
        }

        // Increment reader count
        reader_count++;
        printf("Reader %d: entered. Reader count: %d\n", id, reader_count);

        pthread_mutex_unlock(&mutex);

        // Critical section - read the resource
        printf("Reader %d: reading shared_counter = %d\n", id, shared_counter);
        usleep(rand() % 1000000); // Simulate reading

        // Exit section
        pthread_mutex_lock(&mutex);

        // Decrement reader count
        reader_count--;
        printf("Reader %d: leaving. Reader count: %d\n", id, reader_count);

        // If this is the last reader, signal writers
        if (reader_count == 0) {
            printf("Last reader signaling writers\n");
            pthread_cond_signal(&readers_done);
        }

        pthread_mutex_unlock(&mutex);

        // Rest for a bit before next read
        usleep(rand() % 2000000);
    }

    pthread_exit(NULL);
}

void *writer(void *arg) {
    int id = *((int *)arg);

    while (running) {
        // Entry section
        pthread_mutex_lock(&mutex);

        // Wait if there are readers or another writer
        while ((reader_count > 0 || writer_active) && running) {
            printf("Writer %d waiting\n", id);
            pthread_cond_wait(&readers_done, &mutex);
        }

        // Set writer as active
        writer_active = 1;
        printf("Writer %d: writing\n", id);

        pthread_mutex_unlock(&mutex);

        // Critical section - modify the resource
        shared_counter++;
        printf("Writer %d: incremented shared_counter to %d\n", id, shared_counter);
        usleep(rand() % 1500000); // Simulate writing

        // Exit section
        pthread_mutex_lock(&mutex);

        // Writer no longer active
        writer_active = 0;
        printf("Writer %d: done writing\n", id);

        // Signal readers and writers
        pthread_cond_broadcast(&writers_done);
        pthread_cond_signal(&readers_done);

        pthread_mutex_unlock(&mutex);

        // Rest for a bit before next write
        usleep(rand() % 3000000);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];
    time_t end_time;

    srand(time(NULL));

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&readers_done, NULL);
    pthread_cond_init(&writers_done, NULL);

    printf("Readers-Writers Simulation\n");
    printf("Number of readers: %d\n", NUM_READERS);
    printf("Number of writers: %d\n", NUM_WRITERS);
    printf("Simulation will run for %d seconds\n", EXPERIMENT_DUR);

    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    end_time = time(NULL) + EXPERIMENT_DUR;
    while (time(NULL) < end_time) {
        sleep(1);
    }

    running = 0;
    printf("Simulation ending...\n");

    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&readers_done);
    pthread_cond_broadcast(&writers_done);
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&readers_done);
    pthread_cond_destroy(&writers_done);

    printf("Final counter value: %d\n", shared_counter);
    printf("Simulation ended\n");
    return 0;
}
