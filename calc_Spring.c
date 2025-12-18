#include "Ce_MPS.h"

void calc_Spring(void){
  int i, j;
  double xij, yij, zij, distance;

  #pragma omp parallel for private(j, xij, yij, zij, distance) schedule(dynamic,64)
  for(i=0; i< Num_of_Particles; i++){
    if(ParticleType[i] == MEMBRANE){
      // spring for streching
      for(j=0; j< 4 ; j++){
        distance = calDistance(i, adjacentParticle[i][j]);
        xij = (Position[adjacentParticle[i][j]*3+X] - Position[i*3+X]) / distance;
        yij = (Position[adjacentParticle[i][j]*3+Y] - Position[i*3+Y]) / distance;
        zij = (Position[adjacentParticle[i][j]*3+Z] - Position[i*3+Z]) / distance;

        Acceleration[i*3+X] += SPRING_CONSTANT * (distance - meanWallDistance) * xij;
        Acceleration[i*3+Y] += SPRING_CONSTANT * (distance - meanWallDistance) * yij;
        Acceleration[i*3+Z] += SPRING_CONSTANT * (distance - meanWallDistance) * zij;
        Acceleration[adjacentParticle[i][j]*3+X] -= SPRING_CONSTANT * (distance - meanWallDistance) * xij;
        Acceleration[adjacentParticle[i][j]*3+Y] -= SPRING_CONSTANT * (distance - meanWallDistance) * yij;
        Acceleration[adjacentParticle[i][j]*3+Z] -= SPRING_CONSTANT * (distance - meanWallDistance) * zij;
      }
    }
  }
}
