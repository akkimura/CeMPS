#include "Ce_MPS.h"

void set_SourceTerm(void){
  int i;
  double n0    = N0_4NumberDensity;
  double gamma = RELAX_COEFF_4_PRESSURE;
  double dt_squared_inv = 1.0 / (dt * dt);
  
  #pragma omp parallel for schedule(dynamic,64)
  for(i=0; i< Num_of_Particles; i++){
    SourceTerm[i] = 0.0;
    
    if(ParticleType[i] == GHOST || ParticleType[i] == DUMMY_WALL) continue;
    if(BoundaryCondition[i] == INNER_PARTICLE){
        double density_ratio = (NumberDensity[i] - n0) / n0;
        SourceTerm[i] = gamma * dt_squared_inv * density_ratio;
    }
  }
}
