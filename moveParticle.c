#include "Ce_MPS.h"

void moveParticle(double Time){
  int i;
  double c_velo;

  c_velo = (2 * (0.5 - 0.2) * MaxWidth) / (3 * SIMULATION_TIME);

  #pragma omp parallel for schedule(dynamic,64)
  for(i=0; i< Num_of_Particles; i++){
    if(ParticleType[i] >= MEMBRANE){
      if(ParticleType[i] == CENTROSOME){
        Velocity[i*3  ] = 0;

        if(Time <= SIMULATION_TIME * 0.50){
          if(Position[i*3+0] < threshold){
            Velocity[i*3  ] = - c_velo;
          } else{
            Velocity[i*3  ] = c_velo;
          }
        } else{
          if(Position[i*3+0] < threshold){
            Velocity[i*3  ] = - ((2 * c_velo) / SIMULATION_TIME) * (SIMULATION_TIME - Time);
          } else{
            Velocity[i*3  ] =   ((2 * c_velo) / SIMULATION_TIME) * (SIMULATION_TIME - Time);
          }
        }

        Velocity[i*3+1] = 0;
        Velocity[i*3+2] = 0;
      } else{
        Velocity[i*3  ] += Acceleration[i*3  ] * dt;
        Velocity[i*3+1] += Acceleration[i*3+1] * dt;
        Velocity[i*3+2] += Acceleration[i*3+2] * dt;
      }

      Position[i*3  ] += Velocity[i*3  ] * dt; 
      Position[i*3+1] += Velocity[i*3+1] * dt; 
      Position[i*3+2] += Velocity[i*3+2] * dt;
    }
    Acceleration[i*3  ] = 0.0;
    Acceleration[i*3+1] = 0.0;
    Acceleration[i*3+2] = 0.0;
  }
  Centro_L -= c_velo * dt;
}
