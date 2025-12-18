#include "Ce_MPS.h"

void writePointData(void){
  int i, j = 0, k = 0;
  char fileName[1024];
  FILE* fp;

  for(i=0; i<Num_of_Particles; i++){
    if(ParticleType[i] >= FLUID){
      sprintf(fileName, "%s/data_%06d.csv", data_path, j);
      fp = fopen(fileName, "aw");
      
      fprintf(fp, "%.11lf, %.11lf, %.11lf, %.11lf, %.11lf, %.11lf\n", Position[i*3+0], Position[i*3+1], Position[i*3+2], Velocity[i*3+0], Velocity[i*3+1], Velocity[i*3+2]);
      
      fclose(fp);
      j++;
    }
  }

  for(i=0; i<Num_of_Particles; i++){
    if(ParticleType[i] == MEMBRANE){
      sprintf(fileName, "%s/wall_%06d.csv", wall_path, k);
      fp = fopen(fileName, "aw");
      
      fprintf(fp, "%.11lf, %.11lf, %.11lf, %.11lf, %.11lf, %.11lf\n", Position[i*3+0], Position[i*3+1], Position[i*3+2], Velocity[i*3+0], Velocity[i*3+1], Velocity[i*3+2]);
      
      fclose(fp);
      k++;
    }
  }

}
