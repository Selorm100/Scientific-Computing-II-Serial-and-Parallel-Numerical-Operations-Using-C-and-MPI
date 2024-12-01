#include <stdio.h>

void matrix_vector(int m, int n, double *matA, double *vecA, double *vecC) {
 
 //row
  for (int i = 0; i < m; ++i) {
    vecC[i] = 0.0;
    //col
    for (int j = 0; j < n; ++j) {
      vecC[i] += matA[i * n + j] * vecA[j];
    }
  }
  
}

void matrix_matrix(int m, int n, double *matA, double *matB, double *matC) {
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      matC[i * n + j] = 0.0;
      for (int k = 0; k < n; ++k) {
        matC[i * n + j] += matA[i * n + k] * matB[k * n + j];
      }
    }
  }
}