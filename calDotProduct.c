#include "Ce_MPS.h"

/* 
   calculate Dot Product of n dim vector x & y
*/

double calDotProduct(double* x, double* y, int n){
  int i;
  /* initialize result */
  double result = 0.0;

  /* calculate */
  #pragma omp parallel for reduction(+:result)
  for(i=0; i<n; i++){
    result += x[i] * y[i];
  }

  return result;
}
