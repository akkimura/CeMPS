#include "Ce_MPS.h"

void excep_Process_4BoundaryCondition(void){
  /*When Dirichlet boundary conditions are absent on the fluid domain, the diagonal entries of the matrix should be augmented to handle this special case. This modification enables the matrix system to be solved even without Dirichlet boundary conditions.*/
  
  verify_BoundaryCondition();
  extendDiagonalTerm(CoefficientMatrix);
}
