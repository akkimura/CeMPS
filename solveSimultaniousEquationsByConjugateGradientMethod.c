#include "Ce_MPS.h"

/* By using Conjugate Gradient Method, find x for A*x = b.

   A : n * n Matrix (Symmetric)
   b : SourceTerm
   x : solution
   n : size of matrix
   max_iter : number of max iteration of CG Method (return the actual number of iteration in calculating)
   eps : threshold of error (return the actual error in calculating)

   Refer : http://goo.gl/mvQGqq (2015/09/18)
*/

void MySpMV(double* y, int* row_index, int* col_index, double* val, double* x, int n);

int solveSimultaniousEquationsByConjugateGradientMethod(double* A, double* b, double* x, int n, int max_iter, double eps){
  int i, j, k;
  double *r, *p;
  double rr0, rr1, alpha, beta, e = 0.0;

  int m = 0;
  int *col_index, *row_index;
  double *val, *ap, *ax;
  
  /* error checking */
  if(n <= 0){
    printf("Error : n < 0\n");
    exit(1);
  }

  /* malloc variables */
  r = (double*)malloc(sizeof(double) * n);
  p = (double*)malloc(sizeof(double) * n);

  col_index = (int*)malloc(sizeof(int) * n * n);
  row_index = (int*)malloc(sizeof(int) * (n+1));
  val = (double*)malloc(sizeof(double) * n * n);
  ap = (double*)malloc(sizeof(double) * n);
  ax = (double*)malloc(sizeof(double) * n);

  /* make the first approximate for x (default 0.0) */
  /*
  for(i=0; i<n; i++){
    x[i] = 1E-9;
  }
  */

  /* initialize variables */
  #pragma omp parallel for
  for(i=0; i<n; i++){
    r[i] = 0.0;
    p[i] = 0.0;
    col_index[i] = 0;
    row_index[i] = 0;
    ap[i] = 0.0;
    ax[i] = 0.0;
  }
  row_index[n] = 0;

  /* convert matrix to CRS style */
  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      if(A[i*n+j] != 0){
        col_index[m] = j;
        val[m] = A[i*n+j];
        m++;
      }
    }
    row_index[i+1] = m;
  }

  /* diagonal preconditioning */
  /*
  for(i=0; i<n; i++){
    for(j=row_index[i]; j<=row_index[i+1]-1; j++){
      val[j] /= d[i];
    }
    b[i] /= d[i];
  }
  */


  /* calculate an error for the first approximate for x */
  MySpMV(ax, row_index, col_index, val, x, n);
  #pragma omp parallel for schedule(dynamic,64)
  for(i=0; i<n; i++){
    if(BoundaryCondition[i] == GHOST_OR_DUMMY) continue;
    r[i] = b[i] - ax[i];
    p[i] = r[i];
  }

  /* calculate dot product for r0 */
  rr0 = calDotProduct(r, r, n);

  /*                      */
  /* solve start! (for k) */
  /*                      */

  for(k=0; k<max_iter; k++){

    /* calculate y = AP */
    MySpMV(ap, row_index, col_index, val, p, n);

    /* calculate alpha = (r * r) / (P * AP) */
    alpha = rr0 / calDotProduct(p, ap, n);

    /* update x and error r */
    #pragma omp parallel for schedule(dynamic,64)
    for(i=0; i<n; i++){
      if(BoundaryCondition[i] == GHOST_OR_DUMMY) continue;
      x[i] += alpha * p[i];
      r[i] -= alpha * ap[i];
    }

    /* calculate (r*r)_(k+1) */
    rr1 = calDotProduct(r, r, n);

    /* check convergence (||r|| < eps) */
    MySpMV(ax, row_index, col_index, val, x, n);
    #pragma omp parallel for schedule(dynamic,64)
    for(i=0; i<n; i++){
      ax[i] = b[i] - ax[i];
    }
//    e = sqrt(calDotProduct(ax, ax, n));
    e = sqrt(rr1);
    if(e < eps){
      k++;
      break;
    }

    /* calculate beta and update P */
    beta = rr1 / rr0;
    #pragma omp parallel for schedule(dynamic,64)
    for(i=0; i<n; i++){
      if(BoundaryCondition[i] == GHOST_OR_DUMMY) continue;
      p[i] = r[i] + beta * p[i];
    }

    /* restore (r*r)_(k+1) for next step */
    rr0 = rr1;
  }

  max_iter = k + 1;
  eps = e;

  free(r);
  free(p);
  free(col_index);
  free(row_index);
  free(val);
  free(ap);
  free(ax);

  //  return eps;
  return max_iter;
}

void MySpMV(double* y, int* row_index, int* col_index, double* val, double* x, int n){
  double s;
  int i, j;

  #pragma omp parallel for private(j, s)
  for(i=0; i<n; i++){
    s = 0.0;
    for(j=row_index[i]; j<=row_index[i+1]-1; j++){
      s += val[j] * x[col_index[j]];
    }
    y[i] = s;
  }
}
