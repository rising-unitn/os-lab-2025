#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100
#define MAX_THREADS 8

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

// implement this function
void multiply_matrices(int A[N][N], int B[N][N], int C[N][N], int num_threads);
// implement this function

int main(int argc, char *argv[]) {
    int A[N][N], B[N][N], C[N][N];
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
    fill_matrix(A);
    fill_matrix(B);

    printf("Matrix A (sample):\n");
    print_matrix(A);
    printf("Matrix B (sample):\n");
    print_matrix(B);

    printf("Multiplying matrices using %d threads...\n", num_threads);

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    multiply_matrices(A, B, C, num_threads);
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Matrix C (result sample):\n");
    print_matrix(C);

    printf("Matrix multiplication completed in %.2f seconds using %d threads.\n",
           cpu_time_used, num_threads);

    return 0;
}
