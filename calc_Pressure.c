#include "Ce_MPS.h"

void calc_Pressure(void){
  int iter = 0;

  calc_NumDens();
  set_BoundaryCondition();
  set_SourceTerm();
  set_Matrix();
  printf("solveCG for Pressure Start\n");
  iter = solveSimultaniousEquationsByConjugateGradientMethod(CoefficientMatrix, SourceTerm, Pressure, Num_of_Particles, Num_of_Particles, 1E-15);
  printf("solveCG for Pressure End (iter: %5d)\n", iter);
  remove_Neg_Pressure();
  set_Min_Pressure();
}
