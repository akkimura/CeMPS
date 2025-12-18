#include "Ce_MPS.h"

int cmp2(const void* p, const void* q){
  if(((triangle_t*)p)->p1 - ((triangle_t*)q)->p1 > 0){
    return 1;
  } else if(((triangle_t*)p)->p1 - ((triangle_t*)q)->p1 < 0){
    return -1;
  } else{
    return 0;
  }
}

void replacePointNum(void){
  int i, j, isExist;
  int num_t_real = 0;
  double dist = 0;

  for(j=0; j<num_p; j++){
    isExist = NO;
    for(i=0; i< Num_of_Particles; i++){
      if(ParticleType[i] != MEMBRANE) continue;
      if((fabs(Position[i*3+X] - pcl[j].x) < DISTANCE_OF_PARTICLES*0.5)  && (fabs(Position[i*3+Y] - pcl[j].y) < DISTANCE_OF_PARTICLES*0.5) && (fabs(Position[i*3+Z] - pcl[j].z) < DISTANCE_OF_PARTICLES*0.5)){
        pcl[j].num = i;
        isExist = YES;
      }
    }
    if(isExist == NO){
      if((pcl[j].x == 0) && (pcl[j].y == 0) && (pcl[j].z == 0)){
        pcl[j].isIgn = YES;
      } else{
        printf("particle not found!\n");
        printf("x = %e, y = %e, z = %e\n", pcl[j].x, pcl[j].y, pcl[j].z);
        exit(1);
      }
    }
  }

  qsort(tri, num_t, sizeof(triangle_t), cmp2);
  for(i=0; i<num_t; i++){
    if((pcl[tri[i].p1].isIgn == YES) || (pcl[tri[i].p2].isIgn == YES) || (pcl[tri[i].p3].isIgn == YES)) continue;

    dist += (calDistance(pcl[tri[i].p1].num, pcl[tri[i].p2].num)) / 2;
    dist += (calDistance(pcl[tri[i].p2].num, pcl[tri[i].p3].num)) / 2;
    dist += (calDistance(pcl[tri[i].p3].num, pcl[tri[i].p1].num)) / 2;
    num_t_real++;
  }

  // calcMeanDistance
  meanWallDistance = dist / num_t_real;
  printf("meanWallDistance = %e\n", meanWallDistance);
}
