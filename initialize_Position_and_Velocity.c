#include "Ce_MPS.h"

void initialize_Position_and_Velocity(int shape){
  int iX, iY, iZ;
  int nX, nY, nZ;
  double x, y, z;
  double center, c_rad, dist_l, dist_r, length;
  int i = 0;
  int flagOfParticleGeneration;
  
  /* size decision */
  center = L_AXIS/2 - S_AXIS/2;
  threshold = 0.0;   // center

  nX = (int)(L_AXIS / DISTANCE_OF_PARTICLES) + 30;
  nY = (int)(S_AXIS / DISTANCE_OF_PARTICLES) + 30;
  nZ = (int)(S_AXIS / DISTANCE_OF_PARTICLES) + 30;

  for(iX=-nX/2; iX<=nX/2; iX++){
    for(iY=-nY/2; iY<=nY/2; iY++){
      for(iZ=-nZ/2; iZ<=nZ/2; iZ++){
        x = DISTANCE_OF_PARTICLES * (double)(iX);
        y = DISTANCE_OF_PARTICLES * (double)(iY);
        z = DISTANCE_OF_PARTICLES * (double)(iZ);
        flagOfParticleGeneration = OFF;

        if(shape == NORMAL){     // NORMAL
          MaxWidth = L_AXIS;
          Centro_L = - MaxWidth * 0.1;
          Centro_R =   MaxWidth * 0.1;
          
          /******** DUMMY WALL ************/
          if(((fabs(x) <= center + EPS) 
                && (pow(y,2) + pow(z,2) <= pow(S_AXIS/2 + 8.0*DISTANCE_OF_PARTICLES + EPS, 2))) 
              || (((fabs(x) > center + EPS) && (fabs(x) <= L_AXIS/2 + 8.0*DISTANCE_OF_PARTICLES + EPS)) 
                && (pow(y,2) + pow(z,2) <= pow(S_AXIS/2 + 8.0*DISTANCE_OF_PARTICLES + EPS, 2) - pow(fabs(x) - center, 2)))){ 
            ParticleType[i] = DUMMY_WALL;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = YES;
          }
	  /*********  WALL ************/
          if(((fabs(x) <= center + EPS) 
                && (pow(y,2) + pow(z,2) <= pow(S_AXIS/2 + 6.0*DISTANCE_OF_PARTICLES + EPS, 2)))
              || (((fabs(x) > center + EPS) && (fabs(x) <= L_AXIS/2 + 6.0*DISTANCE_OF_PARTICLES + EPS)) 
                && (pow(y,2) + pow(z,2) <= pow(S_AXIS/2 + 6.0*DISTANCE_OF_PARTICLES + EPS, 2) - pow(fabs(x) - center, 2)))){
            ParticleType[i] = WALL;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = YES;
          }
	  /******* OUTER FLUID  ************/	  
          if(((fabs(x) <= center + EPS) 
                && (pow(y,2) + pow(z,2) <= pow(S_AXIS/2 + 4.0*DISTANCE_OF_PARTICLES + EPS, 2)))
              || (((fabs(x) > center + EPS) && (fabs(x) <= L_AXIS/2 + 4.0*DISTANCE_OF_PARTICLES + EPS)) 
                && (pow(y,2) + pow(z,2) <= pow(S_AXIS/2 + 4.0*DISTANCE_OF_PARTICLES + EPS, 2) - pow(fabs(x) - center, 2)))){
            ParticleType[i] = FLUID_OUT;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = YES;
          }
	  /******* MEMBRANE  ************/	  	  
          if(((fabs(x) <= center + EPS) 
                && (pow(y,2) + pow(z,2) <= pow(S_AXIS/2 + 1.0*DISTANCE_OF_PARTICLES + EPS, 2)))
              || (((fabs(x) > center + EPS) && (fabs(x) <= L_AXIS/2 + 1.0*DISTANCE_OF_PARTICLES + EPS)) 
                && (pow(y,2) + pow(z,2) <= pow(S_AXIS/2 + 1.0*DISTANCE_OF_PARTICLES, 2) - pow(fabs(x) - center, 2)))){
            ParticleType[i] = MEMBRANE;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = NO;
          }
	  /******* FLUID  ************/	  
          if(((fabs(x) <= center + EPS) 
                && (pow(y,2) + pow(z,2) <= pow(S_AXIS/2 + EPS, 2)))
              || (((fabs(x) > center + EPS) && (fabs(x) <= L_AXIS/2 + EPS)) 
                && (pow(y,2) + pow(z,2) <= pow(S_AXIS/2, 2) - pow(fabs(x) - center, 2)))){
            ParticleType[i] = FLUID;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = NO;
          }
        } else if(shape == CIRCLE){     // CIRCLE
          length = sqrt((S_AXIS*(L_AXIS - S_AXIS) + M_PI*pow(S_AXIS/2, 2)) / M_PI);
          MaxWidth = length * 2;
          Centro_L = - MaxWidth * 0.1;
          Centro_R =   MaxWidth * 0.1;

          /******** DUMMY WALL ************/
          if((fabs(x) <= length + 6.0*DISTANCE_OF_PARTICLES) && (fabs(y) <= length + 6.0*DISTANCE_OF_PARTICLES) && (fabs(z) <= length + 6.0*DISTANCE_OF_PARTICLES)){
            ParticleType[i] = DUMMY_WALL;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = YES;
          }
	  /******* OUTER FLUID  ************/	  	  
          if((fabs(x) <= length + 4.0*DISTANCE_OF_PARTICLES) && (fabs(y) <= length + 4.0*DISTANCE_OF_PARTICLES) && (fabs(z) <= length + 4.0*DISTANCE_OF_PARTICLES)){
            ParticleType[i] = FLUID_OUT;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = YES;
          }
	  /*********  WALL ************/	  
          if(pow(x / (length + 1.0*DISTANCE_OF_PARTICLES), 2) + pow(y / (length + 1.0*DISTANCE_OF_PARTICLES), 2) + pow(z / (length + 1.0*DISTANCE_OF_PARTICLES), 2) <= 1 + EPS){
            ParticleType[i] = WALL;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = NO;
          }
	  /******* FLUID  ************/	  	  
          if(pow(x / length, 2) + pow(y / length, 2) + pow(z / length, 2) <= 1 + EPS){
            ParticleType[i] = FLUID;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = NO;
          }
        } else if(shape == TRIANGLE){     // TRIANGLE
          MaxWidth = L_AXIS / 2;

          Centro_L = -MaxWidth * 0.1;
          Centro_R =  MaxWidth * 0.1;

          /******** DUMMY WALL ************/
          if((fabs(x) <= L_AXIS/2 + 8.0*DISTANCE_OF_PARTICLES + EPS) && (fabs(y) <= (-(x/4) + 6.875E-6 + 8.0*DISTANCE_OF_PARTICLES + EPS) && (fabs(z) <= S_AXIS/2 + 8.0*DISTANCE_OF_PARTICLES))){
            ParticleType[i] = DUMMY_WALL;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = YES;
          }
	  /*********  WALL ************/	  	  
          if((fabs(x) <= L_AXIS/2 + 6.0*DISTANCE_OF_PARTICLES + EPS) && (fabs(y) <= (-(x/4) + 6.875E-6 + 6.0*DISTANCE_OF_PARTICLES + EPS) && (fabs(z) <= S_AXIS/2 + 6.0*DISTANCE_OF_PARTICLES))){
            ParticleType[i] = WALL;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = YES;
          }
	  /******* OUTER FLUID  ************/	  	  	  
          if((fabs(x) <= L_AXIS/2 + 4.0*DISTANCE_OF_PARTICLES + EPS) && (fabs(y) <= (-(x/4) + 6.875E-6 + 4.0*DISTANCE_OF_PARTICLES + EPS) && (fabs(z) <= S_AXIS/2 + 4.0*DISTANCE_OF_PARTICLES))){
            ParticleType[i] = FLUID_OUT;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = YES;
          }
	  /******* MEMBRANE  ************/	  	  	  
          if((fabs(x) <= L_AXIS/2 + 1.0*DISTANCE_OF_PARTICLES + EPS) && (fabs(y) <= (-(x/4) + 6.875E-6 + 1.0*DISTANCE_OF_PARTICLES + EPS) && (fabs(z) <= S_AXIS/2 + 1.0*DISTANCE_OF_PARTICLES + EPS))){
            ParticleType[i] = MEMBRANE;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = NO;
          }
	  /******* FLUID  ************/	  	  	  
          if((fabs(x) <= L_AXIS/2 + EPS) && (fabs(y) <= (-(x/4) + 6.875E-6 + EPS) && (fabs(z) <= S_AXIS/2 + EPS))){
            ParticleType[i] = FLUID;
            flagOfParticleGeneration = ON;
            isOutOfCell[i] = NO;
          }

          x += 10.0E-6;
        }

	/******* CENTROSOME  ************/	  	  	  
        c_rad = RADIUS_FOR_CENTROSOME;

        if(asym == ON){
          Centro_L += 0.05 * MaxWidth;
          Centro_R += 0.05 * MaxWidth;
        }        

        dist_l = sqrt(pow(Centro_L - x, 2) + pow(y, 2) + pow(z, 2));
        dist_r = sqrt(pow(Centro_R - x, 2) + pow(y, 2) + pow(z, 2));

        if((dist_l <= c_rad + EPS) || (dist_r <= c_rad + EPS)){
          ParticleType[i] = CENTROSOME;
          flagOfParticleGeneration = ON;
        }

	/*     Particle making    */
        if(flagOfParticleGeneration == ON){
          Position[i*3+0] = x;
          Position[i*3+1] = y;
          Position[i*3+2] = z;
          i++;
        }
      }
    }
  }

  Num_of_Particles = i;

  for(i=0; i<Num_of_Particles*3 ; i++){
    Velocity[i] = 0;
  }

  if(asym == ON){
    threshold += 0.05 * MaxWidth;
  }
}
