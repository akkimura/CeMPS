#include "Ce_MPS.h"

void inputAdjacentParticle(char* filename){
  FILE* fp = fopen(filename, "r");
  int i, tmpv;
  char tmp[256], tmp2[256];

  if(fp == NULL){
    printf("file not found\n");
    exit(1);
  }

  for(i=0; i<4; i++){
    fgets(tmp, 256, fp);
  }
  fscanf(fp, "%s %d %s", tmp, &num_p, tmp2);

  pcl = (particle_t*)malloc(sizeof(particle_t)*num_p);
  printf("num_p = %d\n", num_p);

  for(i=0; i<num_p; i++){
    fscanf(fp, "%lf %lf %lf", &pcl[i].x, &pcl[i].y, &pcl[i].z);
    pcl[i].num = i;
  }
  
  for(i=0; i<num_p+4; i++){
    fgets(tmp, 256, fp);
  }

  fscanf(fp, "%s %d %d", tmp, &num_t, &tmpv);
  tri = (triangle_t*)malloc(sizeof(triangle_t)*num_t);

  for(i=0; i<num_t; i++){
    fscanf(fp, "%d %d %d %d", &tmpv, &tri[i].p1, &tri[i].p2, &tri[i].p3);
    if(tri[i].p1 > tri[i].p2){
      tmpv = tri[i].p1;
      tri[i].p1 = tri[i].p2;
      tri[i].p2 = tmpv;
    }
    if(tri[i].p1 > tri[i].p3){
      tmpv = tri[i].p1;
      tri[i].p1 = tri[i].p3;
      tri[i].p3 = tmpv;
    }
    if(tri[i].p2 > tri[i].p3){
      tmpv = tri[i].p2;
      tri[i].p2 = tri[i].p3;
      tri[i].p3 = tmpv;
    }
  }

  fclose(fp);
}
