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
    long long sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

// Parallel summing using #pragma omp parallel with manual work division
long long sum_parallel_manual(int *arr, int size) {
    long long sum = 0;
    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        int chunk_size = size / num_threads;
        int start = thread_id * chunk_size;
        int end = (thread_id == num_threads - 1) ? size : start + chunk_size;

        long long local_sum = 0;
        for (int i = start; i < end; i++) {
            local_sum += arr[i];
        }

        sum += local_sum;
    }
    return sum;
}

long long sum_parallel_sections(int *arr, int size) {
    long long sum = 0;
    long long partial_sums[NUM_SECTIONS] = {0};
    int chunk_size = size / NUM_SECTIONS;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            for (int i = 0; i < chunk_size; i++) {
                partial_sums[0] += arr[i];
            }
        }

        #pragma omp section
        {
            for (int i = chunk_size; i < 2 * chunk_size; i++) {
                partial_sums[1] += arr[i];
            }
        }

        #pragma omp section
        {
            for (int i = 2 * chunk_size; i < 3 * chunk_size; i++) {
                partial_sums[2] += arr[i];
            }
        }

        #pragma omp section
        {
            for (int i = 3 * chunk_size; i < size; i++) {
                partial_sums[3] += arr[i];
            }
        }
    }

    for (int i = 0; i < NUM_SECTIONS; i++) {
        sum += partial_sums[i];
    }

    return sum;
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
