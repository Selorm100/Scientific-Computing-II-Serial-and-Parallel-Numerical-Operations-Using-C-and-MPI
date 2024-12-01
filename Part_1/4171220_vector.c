#include <stdio.h>

double vector_vector(int m, int n, double *vecA, double *vecB) {

  double result = 0.0;
  for (int i = 0; i < m; ++i) {
    result += vecA[i] * vecB[i];
  }
  return result;
}