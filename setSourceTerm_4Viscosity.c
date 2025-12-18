#include "Ce_MPS.h"

void set_SourceTerm_4Viscosity(void){
  int i;

  #pragma omp parallel for schedule(dynamic,64)
  for(i=0; i< Num_of_Particles; i++){
    SourceTerm_vx[i] = 0.0;
    SourceTerm_vy[i] = 0.0;
    SourceTerm_vz[i] = 0.0;
    if(ParticleType[i] == GHOST || ParticleType[i] == DUMMY_WALL) continue;
    SourceTerm_vx[i] = Velocity[i*3+0];
    SourceTerm_vy[i] = Velocity[i*3+1];
    SourceTerm_vz[i] = Velocity[i*3+2];
  }
}
