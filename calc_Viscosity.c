#include "Ce_MPS.h"

static int solve_viscosity_component(double *source, double *result) {
    return solveSimultaniousEquationsByConjugateGradientMethod(CoefficientMatrix, source, result, Num_of_Particles, Num_of_Particles, 1E-15);
}

void calc_Viscosity(void){
  int iter_x, iter_y, iter_z;
  
  calc_NumDens();
  set_BoundaryCondition();
  set_SourceTerm_4Viscosity();
  set_Matrix_4Viscosity();

  iter_x = solve_viscosity_component(SourceTerm_vx, Viscosity_x);
  printf("Viscosity x calculated (iter: %5d)\n", iter_x);
  
  iter_y = solve_viscosity_component(SourceTerm_vy, Viscosity_y);
  printf("Viscosity y calculated (iter: %5d)\n", iter_y);
  
  iter_z = solve_viscosity_component(SourceTerm_vz, Viscosity_z);
  printf("Viscosity z calculated (iter: %5d)\n", iter_z);  

  #pragma omp parallel for
  for(int i=0; i<Num_of_Particles; i++){
    Velocity[i*3+X] = Viscosity_x[i];
    Velocity[i*3+Y] = Viscosity_y[i];
    Velocity[i*3+Z] = Viscosity_z[i];
  }
}
