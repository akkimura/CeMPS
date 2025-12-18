#include "Ce_MPS.h"

void set_Min_Pressure(void){
  double distance;
  int i, j;

  int ix, iy, iz;
  int jx, jy, jz, jb;

  #pragma omp parallel for private(j, distance, ix, iy, iz, jx, jy, jz, jb) schedule(dynamic,64)
  for(i=0; i< Num_of_Particles; i++){
    if(ParticleType[i] == GHOST || ParticleType[i] == DUMMY_WALL) continue;
    MinimumPressure[i] = Pressure[i];

    ix = (int)((Position[i*3+X] - MIN_X) / BucketSize) + 1;
    iy = (int)((Position[i*3+Y] - MIN_Y) / BucketSize) + 1;
    iz = (int)((Position[i*3+Z] - MIN_Z) / BucketSize) + 1;

    for(jz=iz-1; jz<=iz+1; jz++){
      for(jy=iy-1; jy<=iy+1; jy++){
        for(jx=ix-1; jx<=ix+1; jx++){
          jb = jz*nBxy + jy*nBx + jx;
          j = bfst[jb];
          if(j == -1) continue;

          while(1){
            if((j != i) && (ParticleType[j] != GHOST)){
              if(ParticleType[j] != DUMMY_WALL){
                distance = calDistance(i, j);
                if(pow(distance, 2) < Re_sq_4Gradient){
                  if(MinimumPressure[i] > Pressure[j]){
                    MinimumPressure[i] = Pressure[j];
                  }
                }
              }
            }
            j = nxt[j];
            if(j == -1) break;
          }
        }
      }
    }
  }
}
