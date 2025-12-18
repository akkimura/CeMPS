#include "Ce_MPS.h"

void collision(void){
  int ix, iy, iz;
  int jx, jy, jz, jb;
  int    j;
  double xij, yij, zij;
  double distance;
  double forceDT; /* force by collision between particles */
  double mi, mj;
  double velocity_ix, velocity_iy, velocity_iz;
  double e = COEFF_OF_RESTITUTION;
  static double VelocityAfterCollision[3* SIZE_OF_ARRAY];

  #pragma omp parallel for
  for(int i=0; i<3*Num_of_Particles; i++){ 
    VelocityAfterCollision[i] = Velocity[i]; 
  }

  #pragma omp parallel for private(j, xij, yij, zij, distance, forceDT, mi, mj, velocity_ix, velocity_iy, velocity_iz, ix, iy, iz, jx, jy, jz, jb) schedule(dynamic,64)
  for(int i=0; i<Num_of_Particles; i++){
    if(ParticleType[i] >= FLUID){
      int idx = i * 3;
      double px = Position[idx + X];
      double py = Position[idx + Y];
      double pz = Position[idx + Z];
      
      ix = (int)((px - MIN_X) / BucketSize) + 1;
      iy = (int)((py - MIN_Y) / BucketSize) + 1;
      iz = (int)((pz - MIN_Z) / BucketSize) + 1;

      mi = Fluid_Dens;
      velocity_ix = Velocity[idx + X];
      velocity_iy = Velocity[idx + Y];
      velocity_iz = Velocity[idx + Z];

      for(jz=iz-1; jz<=iz+1; jz++){
        for(jy=iy-1; jy<=iy+1; jy++){
          for(jx=ix-1; jx<=ix+1; jx++){
            jb = jz*nBxy + jy*nBx + jx;
            j = bfst[jb];
            if(j == -1) continue;
            
            while(1){
              if((j != i) && (ParticleType[j] != GHOST)){
		int jidx = j * 3;
                xij = Position[jidx + X] - px;
                yij = Position[jidx + Y] - py;
                zij = Position[jidx + Z] - pz;
                distance = calDistance(i, j);
                if(pow(distance, 2) < Dis_of_collision_sq){
                  forceDT =
		      (velocity_ix - Velocity[jidx + X]) * (xij / distance)
                    + (velocity_iy - Velocity[jidx + Y]) * (yij / distance)
                    + (velocity_iz - Velocity[jidx + Z]) * (zij / distance);
                  if(forceDT > 0.0){
                    mj = Fluid_Dens;
                    forceDT *= (1.0 + e) * mi * mj / (mi + mj);
                    velocity_ix -= (forceDT / mi) * (xij / distance); 
                    velocity_iy -= (forceDT / mi) * (yij / distance); 
                    velocity_iz -= (forceDT / mi) * (zij / distance);
                    /*
                       if(j > i){fprintf(stderr, "WARNING: Collision occured between %d and %d particles.\n", i, j);}
                       */
                  }
                }
              }
              j = nxt[j];
              if(j == -1) break;
            }
          }
        }
      }
      VelocityAfterCollision[idx + X] = velocity_ix;
      VelocityAfterCollision[idx + Y] = velocity_iy;
      VelocityAfterCollision[idx + Z] = velocity_iz;
    }
  }

  #pragma omp parallel for schedule(dynamic,64)
  for(int i=0; i<Num_of_Particles; i++){
    if(ParticleType[i] >= FLUID){
      int idx = i * 3;
      Position[i*3+X] += (VelocityAfterCollision[idx + X] - Velocity[idx + X]) * dt; 
      Position[i*3+Y] += (VelocityAfterCollision[idx + Y] - Velocity[idx + Y]) * dt; 
      Position[i*3+Z] += (VelocityAfterCollision[idx + Z] - Velocity[idx + Z]) * dt;

      Velocity[i*3+X] = VelocityAfterCollision[idx + X];
      Velocity[i*3+Y] = VelocityAfterCollision[idx + Y]; 
      Velocity[i*3+Z] = VelocityAfterCollision[idx + Z];
    }
  }
}

