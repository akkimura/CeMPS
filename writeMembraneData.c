#include "Ce_MPS.h"

void writeMembraneData(void){
  int i;
  char fileName[1024];
  FILE* fp;

  sprintf(fileName, "%s/membrane_%04d.csv", mem_path, Num_of_File);
  fp = fopen(fileName, "w");

  for(i=0; i< Num_of_Particles; i++){
    if(ParticleType[i] == MEMBRANE){
      fprintf(fp, "%.11lf, %.11lf, %.11lf, %.11lf, %.11lf, %.11lf\n", Position[i*3+0], Position[i*3+1], Position[i*3+2], Velocity[i*3+0], Velocity[i*3+1], Velocity[i*3+2]);
    }
  }

  fclose(fp);

  writeMesh();
}
