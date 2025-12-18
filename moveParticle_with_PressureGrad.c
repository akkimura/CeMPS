#include "Ce_MPS.h"

void moveParticle_with_PressureGrad(void){
  #pragma omp parallel for schedule(dynamic,64)
  for(int i=0; i< Num_of_Particles; i++){
    int idx = i * 3;
    
    if((ParticleType[i] >= MEMBRANE) && (ParticleType[i] != CENTROSOME)){
      for(int k = 0; k < 3; k++){
	Velocity[idx + k] += Acceleration[idx + k] * dt;
	Position[idx + k] += Acceleration[idx + k] * dt * dt;
      }
    }
    Acceleration[i*3  ] = 0.0;
    Acceleration[i*3+1] = 0.0;
    Acceleration[i*3+2] = 0.0;
  }
  printf("x max = %e\n", findMaxValue(Position, 1));
  printf("y max = %e\n", findMaxValue(Position, 2));
  printf("z max = %e\n", findMaxValue(Position, 3));
}
