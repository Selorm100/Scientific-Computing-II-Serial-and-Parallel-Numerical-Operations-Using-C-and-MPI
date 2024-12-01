#include "4171220_matrix.h"
#include "4171220_vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 15 // Width of each column

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <matrix_size>\n", argv[0]);
    printf("On the command line, enter the 'N' argument\n");
    return 1;
  }

  int N = atoi(argv[1]);
  FILE *file = fopen("4171220_serial.txt", "w");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  // Write table header to the file
  fprintf(file, "%-*s %-*s %-*s %-*s\n", WIDTH, "N", WIDTH, "V.V/s", WIDTH,
          "M.V/s", WIDTH, "M.M/s");

  // 4 Points
  for (int i = 0; i < 4; i++) {
    double *vecA = (double *)malloc(N * sizeof(double));
    double *vecB = (double *)malloc(N * sizeof(double));
    double *matA = (double *)malloc(N * N * sizeof(double));
    double *matB = (double *)malloc(N * N * sizeof(double));
    double *vecC = (double *)malloc(N * sizeof(double));
    double *matC = (double *)malloc(N * N * sizeof(double));

    // Initialize the vectors and matrices (populating with random)
    for (int j = 0; j < N; ++j) {
      vecA[j] = rand() / (double)RAND_MAX;
      vecB[j] = rand() / (double)RAND_MAX;
      for (int k = 0; k < N; ++k) {
        matA[j * N + k] = rand() / (double)RAND_MAX;
        matB[j * N + k] = rand() / (double)RAND_MAX;
      }
    }

    fprintf(file, "%-*d", WIDTH, N);

    struct timespec start, end;

    // Timing vector-vector multiplication
    clock_gettime(CLOCK_MONOTONIC, &start);
    double vt = vector_vector(N, N, vecA, vecB);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_time = (end.tv_sec - start.tv_sec) + 
                          (end.tv_nsec - start.tv_nsec) / 1E9;
    fprintf(file, " %-*f", WIDTH, elapsed_time);

    // Timing matrix-vector multiplication
    clock_gettime(CLOCK_MONOTONIC, &start);
    matrix_vector(N, N, matA, vecA, vecC);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = (end.tv_sec - start.tv_sec) + 
                   (end.tv_nsec - start.tv_nsec) / 1E9;
    fprintf(file, " %-*f", WIDTH, elapsed_time);

    // Timing matrix-matrix multiplication
    clock_gettime(CLOCK_MONOTONIC, &start);
    matrix_matrix(N, N, matA, matB, matC);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = (end.tv_sec - start.tv_sec) + 
                   (end.tv_nsec - start.tv_nsec) / 1E9;
    fprintf(file, " %-*f\n", WIDTH, elapsed_time);

    N *= 2;

    free(vecA);
    free(vecB);
    free(matA);
    free(matB);
    free(vecC);
    free(matC);
  }

  fclose(file);

  return 0;
}

