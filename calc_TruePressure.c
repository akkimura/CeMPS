#include "Ce_MPS.h"

static void set_SourceTerm_4TruePressure(void){
  int i, j;
  double distance, w;
  double dx, dy, dz;
  double dgx, dgy, dgz;
  double div_g;
  double element_a = 3.0 / N0_4Gradient;

  int ix, iy, iz;
  int jx, jy, jz, jb;

  #pragma omp parallel for
  for(i=0; i<Num_of_Particles; i++){
    TruePressureGrad[i*3+X] = MASS_DENSITY * (Viscosity_x[i] - VelocityBeforeViscosity[i*3+X]) / dt;
    TruePressureGrad[i*3+Y] = MASS_DENSITY * (Viscosity_y[i] - VelocityBeforeViscosity[i*3+Y]) / dt;
    TruePressureGrad[i*3+Z] = MASS_DENSITY * (Viscosity_z[i] - VelocityBeforeViscosity[i*3+Z]) / dt;
    SourceTerm[i] = 0.0;
  }

  #pragma omp parallel for private(j, distance, w, dx, dy, dz, dgx, dgy, dgz, div_g, ix, iy, iz, jx, jy, jz, jb) schedule(dynamic,64)
  for(i=0; i<Num_of_Particles; i++){
    if(ParticleType[i] == GHOST || ParticleType[i] == DUMMY_WALL) continue;

    ix = (int)((Position[i*3+X] - MIN_X) / BucketSize) + 1;
    iy = (int)((Position[i*3+Y] - MIN_Y) / BucketSize) + 1;
    iz = (int)((Position[i*3+Z] - MIN_Z) / BucketSize) + 1;

    div_g = 0.0;
    for(jz=iz-1; jz<=iz+1; jz++){
      for(jy=iy-1; jy<=iy+1; jy++){
        for(jx=ix-1; jx<=ix+1; jx++){
          jb = jz*nBxy + jy*nBx + jx;
          j = bfst[jb];
          if(j == -1) continue;

          while(1){
            if((j != i) && (ParticleType[j] != GHOST) && (ParticleType[j] != DUMMY_WALL)){
              distance = calDistance(i, j);
              if(distance < Re_4Gradient){
                dx = Position[j*3+X] - Position[i*3+X];
                dy = Position[j*3+Y] - Position[i*3+Y];
                dz = Position[j*3+Z] - Position[i*3+Z];
                dgx = TruePressureGrad[j*3+X] - TruePressureGrad[i*3+X];
                dgy = TruePressureGrad[j*3+Y] - TruePressureGrad[i*3+Y];
                dgz = TruePressureGrad[j*3+Z] - TruePressureGrad[i*3+Z];
                w = weight(distance, Re_4Gradient);
                div_g += ((dgx * dx + dgy * dy + dgz * dz) / (distance * distance)) * w;
              }
            }
            j = nxt[j];
            if(j == -1) break;
          }
        }
      }
    }
    SourceTerm[i] = (-1.0) * element_a * div_g;
  }
}

static void set_Matrix_4TruePressure(void){
  int i, j;
  double distance, coeff_IJ;
  double n0 = N0_4Laplacian;
  double element_a = 2.0 * 3.0 / (n0 * Lambda);
  int n = Num_of_Particles;

  int ix, iy, iz;
  int jx, jy, jz, jb;

  #pragma omp parallel for private(j)
  for(i=0; i<Num_of_Particles; i++){
    for(j=0; j<Num_of_Particles; j++){
      CoefficientMatrix[i*n+j] = 0.0;
    }
  }

  #pragma omp parallel for private(j, distance, coeff_IJ, ix, iy, iz, jx, jy, jz, jb) schedule(dynamic,64)
  for(i=0; i<Num_of_Particles; i++){
    if(ParticleType[i] == GHOST || ParticleType[i] == DUMMY_WALL) continue;

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
              distance = calDistance(i, j);
              if(distance < Re_4Laplacian){
                coeff_IJ = element_a * weight(distance, Re_4Laplacian);
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
  }
  excep_Process_4BoundaryCondition();
}

static void remove_Mean_TruePressure(void){
  int i, count = 0;
  double sum = 0.0;

  for(i=0; i<Num_of_Particles; i++){
    if(ParticleType[i] == GHOST || ParticleType[i] == DUMMY_WALL) continue;
    sum += TruePressure[i];
    count++;
  }
  if(count == 0) return;

  double mean = sum / (double)count;
  for(i=0; i<Num_of_Particles; i++){
    if(ParticleType[i] == GHOST || ParticleType[i] == DUMMY_WALL){
      TruePressure[i] = 0.0;
    } else{
      TruePressure[i] -= mean;
    }
  }
}

void calc_TruePressure(void){
  int iter;

  set_SourceTerm_4TruePressure();
  set_Matrix_4TruePressure();

  #pragma omp parallel for
  for(int i=0; i<Num_of_Particles; i++){
    TruePressure[i] = 0.0;
  }

  printf("solveCG for TruePressure Start\n");
  iter = solveSimultaniousEquationsByConjugateGradientMethod(CoefficientMatrix, SourceTerm, TruePressure, Num_of_Particles, Num_of_Particles, 1E-15);
  printf("solveCG for TruePressure End (iter: %5d)\n", iter);
  remove_Mean_TruePressure();
}
