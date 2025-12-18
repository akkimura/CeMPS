#include "Ce_MPS.h"

void calculateGravity(void){
  int i, idx;
  #pragma omp parallel for private(idx)
  for(i=0; i< Num_of_Particles; i++){
    idx = i * 3;
    if(ParticleType[i] >= FLUID){
      Acceleration[idx+X] = G_X;
      Acceleration[idx+Y] = G_Y;
      Acceleration[idx+Z] = G_Z;
    } else{
      Acceleration[idx+X] = 0.0;
      Acceleration[idx+Y] = 0.0;
      Acceleration[idx+Z] = 0.0;
    }
  }
}
