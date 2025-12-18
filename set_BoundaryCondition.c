#include "Ce_MPS.h"

void set_BoundaryCondition(void){
  int i;
  double threshold = THRESH_RATIO_OF_NUM_DENS * N0_4NumberDensity;
  
 #pragma omp parallel for schedule(dynamic,64)
  for(i=0; i< Num_of_Particles; i++){
    if(ParticleType[i] == GHOST || ParticleType[i] == DUMMY_WALL){
      BoundaryCondition[i] = GHOST_OR_DUMMY;
    } else if(NumberDensity[i] < threshold){
      BoundaryCondition[i] = SURFACE_PARTICLE;
    } else{
      BoundaryCondition[i] = INNER_PARTICLE;
    }
  }
}
