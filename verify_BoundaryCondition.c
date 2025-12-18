#include "Ce_MPS.h"

void verify_BoundaryCondition(void){
  int j, count;
  double distance;

  int ix, iy, iz;
  int jx, jy, jz, jb;

  #pragma omp parallel for schedule(dynamic,64)
  for(int i=0; i<Num_of_Particles; i++){
    if(BoundaryCondition[i] == GHOST_OR_DUMMY){
      FlagForCheckingBoundaryCondition[i] = GHOST_OR_DUMMY;
    } else if(BoundaryCondition[i] == SURFACE_PARTICLE){
      FlagForCheckingBoundaryCondition[i] = FLAG_DIRICHLET_BOUNDARY_IS_CONNECTED;
    } else{
      FlagForCheckingBoundaryCondition[i] = FLAG_DIRICHLET_BOUNDARY_IS_NOT_CONNECTED;
    }
  }

  do{
    count = 0;
    for(int i=0; i<Num_of_Particles; i++){
      if(FlagForCheckingBoundaryCondition[i] == FLAG_DIRICHLET_BOUNDARY_IS_CONNECTED){
        ix = (int)((Position[i*3+X] - MIN_X) / BucketSize) + 1;
        iy = (int)((Position[i*3+Y] - MIN_Y) / BucketSize) + 1;
        iz = (int)((Position[i*3+Z] - MIN_Z) / BucketSize) + 1;

        for(jz=iz-1; jz<=iz+1; jz++){
          for(jy=iy-1; jy<=iy+1; jy++){
            for(jx=ix-1; jx<=ix+1; jx++){
              jb = jz*nBxy + jy*nBx+ jx;
              j = bfst[jb];
              if(j == -1) continue;

              while(1){
                if((j != i) && (ParticleType[j] != GHOST) && (ParticleType[j] != DUMMY_WALL)){
                  if(FlagForCheckingBoundaryCondition[j] == FLAG_DIRICHLET_BOUNDARY_IS_NOT_CONNECTED){
                    distance = calDistance(i, j);
                    if(pow(distance, 2) < Re_sq_4Laplacian){
                      FlagForCheckingBoundaryCondition[j] = FLAG_DIRICHLET_BOUNDARY_IS_CONNECTED;
                    }
                  }
                }
                j = nxt[j];
                if(j == -1) break;
              }
            }
          }
        }
        FlagForCheckingBoundaryCondition[i] = FLAG_DIRICHLET_BOUNDARY_IS_CHECKED;
        count++;
      }
    }
  } while(count != 0);
  /* Repeat until all fluid or wall particles with Dirichlet boundary conditions in the particle group reach the "FLAG_DIRICHLET_BOUNDARY_IS_CHECKED" state. */

  #pragma omp parallel for
  for(int i=0; i<Num_of_Particles; i++){
    if(FlagForCheckingBoundaryCondition[i] == FLAG_DIRICHLET_BOUNDARY_IS_NOT_CONNECTED){
      //      fprintf(stderr, "WARNING: There is no dirichlet boundary condition for %d-th particle.\n", i);
    }
  }
}

