#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

double vector_vector(int m, int n, double *vecA, double *vecB) {
    double local_sum = 0.0;

    // Perform local computation
    for (int i = 0; i < m; i++) {
        local_sum += vecA[i] * vecB[i];
    }

    return local_sum;
}
