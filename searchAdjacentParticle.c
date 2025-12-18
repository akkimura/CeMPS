#include "Ce_MPS.h"

int cmp(const void* p, const void* q){
  if(((particle_t*)p)->dist - ((particle_t*)q)->dist > 0){
    return 1;
  } else if(((particle_t*)p)->dist - ((particle_t*)q)->dist < 0){
    return -1;
  } else{
    return 0;
  }
}

void searchAdjacentParticle(void){
  int i, j, num = 0;
  double sum = 0;
  particle_t* p;

  p = (particle_t*)malloc(sizeof(particle_t)*Num_of_Particles);

  for(i=0; i<Num_of_Particles; i++){
    if(ParticleType[i] == MEMBRANE){
      // calc distance
      for(j=0; j<Num_of_Particles; j++){
        p[j].num = j;
        if(ParticleType[j] == MEMBRANE){
          p[j].dist = calDistance(i, j);
        } else{
          p[j].dist = 1E7;
        }
      }

      // sort
      qsort(p, Num_of_Particles, sizeof(particle_t), cmp);

      // decide
      for(j=0; j< 4 ; j++){
        adjacentParticle[i][j] = p[j+1].num;
      }
    }
  }

  // calc meanWallDistance
  for(i=0; i<Num_of_Particles; i++){
    if(ParticleType[i] == MEMBRANE){
      for(j=0; j< 4 ; j++){
        sum += calDistance(i, adjacentParticle[i][j]);
        num++;
      }
    }
  }
  meanWallDistance = sum / num;

  free(p);
}
