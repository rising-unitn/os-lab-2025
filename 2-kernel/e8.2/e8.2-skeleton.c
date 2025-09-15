#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 100000000
#define NUM_SECTIONS 4

double get_time() {
    return omp_get_wtime();
}

long long sum_sequential(int *arr, int size) {
    long long sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

// Parallel summing using #pragma omp parallel for
long long sum_parallel_for(int *arr, int size) {
    // #pragma omp parallel for reduction(+:sum)
}

// Parallel summing using #pragma omp parallel with manual work division
long long sum_parallel_manual(int *arr, int size) {
}

long long sum_parallel_sections(int *arr, int size) {
}

int main(int argc, char *argv[]) {
    int num_threads = 10;

    if (argc > 1) {
        num_threads = atoi(argv[1]);
        if (num_threads <= 0) {
            printf("Invalid number of threads. Using default (4).\n");
            num_threads = 4;
        }
    }

    printf("Using %d threads\n", num_threads);
    omp_set_num_threads(num_threads);

    printf("Initializing array with %d elements...\n", ARRAY_SIZE);
    int *arr = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (!arr) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = rand() % 100 + 1;
    }

    double start_time, end_time;
    long long result;

    printf("\n1. Sequential summing:\n");
    start_time = get_time();
    result = sum_sequential(arr, ARRAY_SIZE);
    end_time = get_time();
    printf("Sum: %lld\n", result);
    printf("Time: %.6f seconds\n", end_time - start_time);

    printf("\n2. Parallel summing with #pragma omp parallel for:\n");
    start_time = get_time();
    result = sum_parallel_for(arr, ARRAY_SIZE);
    end_time = get_time();
    printf("Sum: %lld\n", result);
    printf("Time: %.6f seconds\n", end_time - start_time);

    printf("\n3. Parallel summing with manual work division:\n");
    start_time = get_time();
    result = sum_parallel_manual(arr, ARRAY_SIZE);
    end_time = get_time();
    printf("Sum: %lld\n", result);
    printf("Time: %.6f seconds\n", end_time - start_time);

    printf("\n4. Parallel summing with #pragma omp sections:\n");
    start_time = get_time();
    result = sum_parallel_sections(arr, ARRAY_SIZE);
    end_time = get_time();
    printf("Sum: %lld\n", result);
    printf("Time: %.6f seconds\n", end_time - start_time);

    free(arr);
    return 0;
}
