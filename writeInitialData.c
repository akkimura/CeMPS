#include "Ce_MPS.h"

void writeInitialData(void){
  FILE* fp;
  int i, n = Num_of_Particles;
  char path[512];
  
  snprintf(path,sizeof(path), "%s/point.dat", dir_path);
  fp = fopen(path, "w");
  for(i=0; i<n; i++){
    if(ParticleType[i] == MEMBRANE){
      fprintf(fp, "%e, %e, %e\n", Position[i*3+X], Position[i*3+Y], Position[i*3+Z]);
    }
  }
  fclose(fp);
}
