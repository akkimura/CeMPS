#include "Ce_MPS.h"

void initializeBucket(void){
  BucketSize = 2.1 * DISTANCE_OF_PARTICLES;
  BucketSize *= 1.1;
  
  nBx = (int)((L_AXIS+16 * DISTANCE_OF_PARTICLES)/BucketSize) + 5;
  nBy = (int)((S_AXIS+16 * DISTANCE_OF_PARTICLES)/BucketSize) + 5;
  nBz = (int)((S_AXIS+16 * DISTANCE_OF_PARTICLES)/BucketSize) + 5;
  nBxy = nBx * nBy;
  nBxyz = nBx * nBy * nBz;
  printf("Finish initializing: nBx=%d, nBy=%d, nBz=%d, nBxyz=%d\n", nBx, nBy, nBz, nBxyz);

  bfst = (int*)malloc(sizeof(int) * nBxyz * EX);
  blst = (int*)malloc(sizeof(int) * nBxyz * EX);
  nxt  = (int*)malloc(sizeof(int) * Num_of_Particles);
}

