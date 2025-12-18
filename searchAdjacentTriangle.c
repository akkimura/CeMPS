#include "Ce_MPS.h"

void searchAdjacentTriangle(void){
  int i, j, k, l, m;
  int p[3], q[3], same;

  adjacentTriangle = (int**)malloc(sizeof(int)*num_t*3);
  for(i=0; i<num_t; i++){
    adjacentTriangle[i] = (int*)malloc(sizeof(int)*3);
    for(j=0; j<3; j++){
      adjacentTriangle[i][j] = -1;
    }
  }

  for(i=0; i<num_t; i++){
    if((pcl[tri[i].p1].isIgn == YES) || (pcl[tri[i].p2].isIgn == YES) || (pcl[tri[i].p3].isIgn == YES)) continue;
    p[0] = tri[i].p1;
    p[1] = tri[i].p2;
    p[2] = tri[i].p3;
    m = 0;
    for(j=0; j<num_t; j++){
      if((pcl[tri[i].p1].isIgn == YES) || (pcl[tri[i].p2].isIgn == YES) || (pcl[tri[i].p3].isIgn == YES)) continue;
      q[0] = tri[j].p1;
      q[1] = tri[j].p2;
      q[2] = tri[j].p3;

      same = 0;
      for(k=0; k<3; k++){
        for(l=0; l<3; l++){
          if(p[k] == q[l]){
            same++;
          }
        }
      }
      if(same == 2){
        adjacentTriangle[i][m] = j;
        m++;
      }
    }
  }
}
