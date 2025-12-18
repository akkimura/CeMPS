#include "Ce_MPS.h"

void makeBucket(void){
  int i, j;
  int ix, iy, iz, ib;

  for(i=0; i<nBxyz; i++){
    bfst[i] = -1;
    blst[i] = -1;
  }
  for(i=0; i<Num_of_Particles; i++){
    nxt[i] = -1;
  }

  for(i=0; i<Num_of_Particles; i++){
    if(ParticleType[i] == GHOST) continue;
      ix = (int)((Position[i*3+X] - MIN_X) / BucketSize) + 1;
      iy = (int)((Position[i*3+Y] - MIN_Y) / BucketSize) + 1;
      iz = (int)((Position[i*3+Z] - MIN_Z) / BucketSize) + 1;
      ib = iz*nBxy + iy*nBx + ix;

      if(ib > nBxyz * EX){
        printf("ib = %d, nBxyz * EX = %d\n", ib, nBxyz * EX);
        exit(1);
      }
      
      j = blst[ib];
      blst[ib] = i;
      if(j == -1){
        bfst[ib] = i;
      } else{
        nxt[j] = i;
      }
  }
}
