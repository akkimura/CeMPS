#include "Ce_MPS.h"

void calc_PressureGrad(void){
  int    i, j;
  double xij, yij, zij;
  double grad_x, grad_y, grad_z;
  double distance;
  double w, pij;
  double element_a;

  int ix, iy, iz;
  int jx, jy, jz, jb;

  element_a = 3 / N0_4Gradient;
  #pragma omp parallel for private(j, xij, yij, zij, grad_x, grad_y, grad_z, distance, w, pij, ix, iy, iz, jx, jy, jz, jb) schedule(dynamic,64)
  for(i=0; i< Num_of_Particles; i++){
    if(ParticleType[i] >= MEMBRANE){
      grad_x = 0.0;  grad_y = 0.0;  grad_z = 0.0;
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
              if((j != i) && (ParticleType[j] != GHOST) && (ParticleType[j] != DUMMY_WALL)){
                xij = Position[j*3+X] - Position[i*3+X];
                yij = Position[j*3+Y] - Position[i*3+Y];
                zij = Position[j*3+Z] - Position[i*3+Z];
                distance = calDistance(i, j);
                if(distance < Re_4Gradient){
                  w = weight(distance, Re_4Gradient);
                  pij = (Pressure[j] - MinimumPressure[i]) / pow(distance, 2);
                  grad_x = grad_x + (xij * pij * w);
                  grad_y = grad_y + (yij * pij * w);
                  grad_z = grad_z + (zij * pij * w);
                }
              }
              j = nxt[j];
              if(j == -1) break;
            }
          }
        }
      }
      grad_x = grad_x * element_a;
      grad_y = grad_y * element_a;
      grad_z = grad_z * element_a;
      Acceleration[i*3+X] = (-1.0) * grad_x / Fluid_Dens;
      Acceleration[i*3+Y] = (-1.0) * grad_y / Fluid_Dens;
      Acceleration[i*3+Z] = (-1.0) * grad_z / Fluid_Dens;
    }
  }
}

