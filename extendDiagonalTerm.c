#include "Ce_MPS.h"

void extendDiagonalTerm(double* Matrix){
  int n = Num_of_Particles;

  #pragma omp parallel for
  for(int i=0; i<n; i++){
    if(FlagForCheckingBoundaryCondition[i] == FLAG_DIRICHLET_BOUNDARY_IS_NOT_CONNECTED){
      Matrix[i*n+i] = 2.0 * Matrix[i*n+i];
    }
  }
}
