#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100
#define MAX_THREADS 8

int A[N][N], B[N][N], C[N][N];
int num_threads_global;

void fill_matrix(int matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void print_matrix(int matrix[N][N]) {
    for (int i = 0; i < N && i < 10; i++) {
        for (int j = 0; j < N && j < 10; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("... (showing only top-left 10x10 submatrix)\n");
}

void *multiply_thread(void *arg) {
    long tid = (long)arg;

    int rows_per_thread = N / num_threads_global;
    int start_row = tid * rows_per_thread;
    int end_row = (tid == num_threads_global - 1) ? N : start_row + rows_per_thread;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

void multiply_matrices(int A_in[N][N], int B_in[N][N], int C_out[N][N], int num_threads) {
    pthread_t threads[MAX_THREADS];

    num_threads_global = num_threads;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = A_in[i][j];
            B[i][j] = B_in[i][j];
        }
    }

    for (long t = 0; t < num_threads; t++) {
        int rc = pthread_create(&threads[t], NULL, multiply_thread, (void *)t);
        if (rc) {
            printf("Error> return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C_out[i][j] = C[i][j];
        }
    }
}

int main(int argc, char *argv[]) {
    int local_A[N][N], local_B[N][N], local_C[N][N];
    int num_threads = 4;

    if (argc > 1) {
        num_threads = atoi(argv[1]);
        if (num_threads <= 0 || num_threads > MAX_THREADS) {
            printf("Invalid number of threads. Using default (4).\n");
            num_threads = 4;
        }
    }

    srand(time(NULL));

    printf("Initializing matrices of size %dx%d...\n", N, N);
    fill_matrix(local_A);
    fill_matrix(local_B);

    printf("Matrix A (sample):\n");
    print_matrix(local_A);
    printf("Matrix B (sample):\n");
    print_matrix(local_B);

    printf("Multiplying matrices using %d threads...\n", num_threads);

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    multiply_matrices(local_A, local_B, local_C, num_threads);
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Matrix C (result sample):\n");
    print_matrix(local_C);

    printf("Matrix multiplication completed in %.2f seconds using %d threads.\n",
           cpu_time_used, num_threads);

    return 0;
}
