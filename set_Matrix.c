#include "Ce_MPS.h"

void set_Matrix(void){
  double distance;
  double coeff_IJ;
  double n0 = N0_4Laplacian;
  int    i, j;
  double element_a;
  int n = Num_of_Particles;

  int ix, iy, iz;
  int jx, jy, jz, jb;

  #pragma omp parallel for private(j)
  for(i=0; i<Num_of_Particles; i++){
    for(j=0; j<Num_of_Particles; j++){
      CoefficientMatrix[i*n+j] = 0.0;
    }
  }

  element_a = 2.0 * 3 / (n0 * Lambda);
  #pragma omp parallel for private(j, distance, coeff_IJ, ix, iy, iz, jx, jy, jz, jb) schedule(dynamic,64)
  for(i=0; i<Num_of_Particles; i++){
    if(BoundaryCondition[i] != INNER_PARTICLE) continue;
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
            if((j != i) && (BoundaryCondition[j] != GHOST_OR_DUMMY)){
              distance = calDistance(i, j);
              if(distance < Re_4Laplacian){
                coeff_IJ = element_a * weight(distance, Re_4Laplacian) / Fluid_Dens;
                CoefficientMatrix[i*n+j]  = (-1.0) * coeff_IJ;
                CoefficientMatrix[i*n+i] += coeff_IJ;
              }
            }
            j = nxt[j];
            if(j == -1) break;
          }
        }
      }
    }
    CoefficientMatrix[i*n+i] += (COMPRESSIBILITY) / (dt * dt);
  }
  excep_Process_4BoundaryCondition();
}

