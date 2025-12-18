#include "Ce_MPS.h"

void calc_NumDens(void){
  int    i, j;
  double distance;
  double w;

  int ix, iy, iz;
  int jx, jy, jz, jb;

  #pragma omp parallel for private(j, distance, w, ix, iy, iz, jx, jy, jz, jb) schedule(dynamic,64)
  for(i=0; i< Num_of_Particles; i++){
    NumberDensity[i] = 0.0;
    if(ParticleType[i] == GHOST) continue;
    ix = (int)((Position[i*3+X] - MIN_X) / BucketSize) + 1;
    iy = (int)((Position[i*3+Y] - MIN_Y) / BucketSize) + 1;
    iz = (int)((Position[i*3+Z] - MIN_Z) / BucketSize) + 1;

    for(jz=iz-1; jz<=iz+1; jz++){
      for(jy=iy-1; jy<=iy+1; jy++){
        for(jx=ix-1; jx<=ix+1; jx++){
          jb = jz*nBxy + jy*nBx + jx;
          if(jb > nBxyz * EX){
            printf("jb = %d, nBxyz * EX = %d\n", jb, nBxyz * EX);
            exit(1);
          }
          j = bfst[jb];
          if(j == -1) continue;

          while(1){
            if((j != i) && (ParticleType[j] != GHOST)){
              distance = calDistance(i, j);
              w = weight(distance, Re_4NumberDensity);
              NumberDensity[i] += w;
            }
            j = nxt[j];
            if(j == -1) break;
          }
        }
      }
    }
  }
}

