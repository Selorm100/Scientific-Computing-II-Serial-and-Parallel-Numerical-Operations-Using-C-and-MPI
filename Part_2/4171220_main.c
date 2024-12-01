#include "4171220_vector.h"
#include "4171220_matrix.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int rank, size;
  FILE *file;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (argc != 2) {
    if (rank == 0) {
      printf("Usage: %s <vector_size>\n", argv[0]);
    }
    MPI_Finalize();
    return 1;
  }

  int N = atoi(argv[1]);

  double *vecA = NULL;
  double *vecB = NULL;
  double local_result = 0.0;
  double total_result = 0.0;

  if (rank == 0) {
    // initialising the output file
    if (size == 1) {
      file = fopen("4171220_scale.txt", "w");
      fprintf(file, "%-9s %-4s %9s\n", "N", "core", "vv/s");
    } else {
      file = fopen("4171220_scale.txt", "a");
    }

    // Memory allocation and population of vectors on root process
    vecA = (double *)malloc(N * sizeof(double));
    vecB = (double *)malloc(N * sizeof(double));

    for (int i = 0; i < N; ++i) {
      vecA[i] = rand() / (double)RAND_MAX;
      vecB[i] = rand() / (double)RAND_MAX;
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);

  // Determine the number of elements each process will handle
  // This part handles uniform or non uniform distributions
  int local_n = (rank < N % size) ? (N / size + 1) : (N / size);
  double *local_vecA = (double *)malloc(local_n * sizeof(double));
  double *local_vecB = (double *)malloc(local_n * sizeof(double));

  // Calculate send counts and displacements
  int *sendcounts = (int *)malloc(size * sizeof(int));
  int *displs = (int *)malloc(size * sizeof(int));

  for (int i = 0; i < size; i++) {
    sendcounts[i] = (i < N % size) ? (N / size + 1) : (N / size);
    displs[i] = (i == 0) ? 0 : (displs[i - 1] + sendcounts[i - 1]);
  }


  MPI_Scatterv(vecA, sendcounts, displs, MPI_DOUBLE, local_vecA, local_n,
               MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatterv(vecB, sendcounts, displs, MPI_DOUBLE, local_vecB, local_n,
               MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Synchronize before starting timing
  MPI_Barrier(MPI_COMM_WORLD);

  double start_time = MPI_Wtime();

  local_result = vector_vector(local_n, local_n, local_vecA, local_vecB);

  MPI_Reduce(&local_result, &total_result, 1, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);

   

  double end_time = MPI_Wtime();
  double total_time = end_time - start_time;

  if (rank == 0) {
    printf("%-9s %-4s %9s\n", "N", "core", "vv/s");
    printf("%9d %4d %9.4f\n", N, size, total_time);
    fprintf(file, "%9d %4d %9.4f\n", N, size, total_time);

    fclose(file);
  }

  free(local_vecA);
  free(local_vecB);
  free(sendcounts);
  free(displs);

  if (rank == 0) {
    free(vecA);
    free(vecB);
  }

  MPI_Finalize();
  return 0;
}
