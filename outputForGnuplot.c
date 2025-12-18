#include "Ce_MPS.h"

void outputForGnuplot(void){
  int i, j;
  FILE* fp;

  fp = fopen("./membrane.dat", "w");
  for(i=0; i< Num_of_Particles; i++){
    if(ParticleType[i] == MEMBRANE){
      for(j=0; j< 4 ; j++){
        fprintf(fp, "%e %e %e\n", Position[i*3+X], Position[i*3+Y], Position[i*3+Z]);
        fprintf(fp, "%e %e %e\n\n", Position[adjacentParticle[i][j]*3+X], Position[adjacentParticle[i][j]*3+Y], Position[adjacentParticle[i][j]*3+Z]);
      }
    }
  }

  fclose(fp);
}
