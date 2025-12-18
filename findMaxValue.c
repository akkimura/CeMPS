#include "Ce_MPS.h"

double findMaxValue(double* x, int d){
  int i, n = Num_of_Particles;
  double max;

  max = 0;
  for(i=0; i<n; i++){
    if(fabs(x[i*3 + (d-1)]) > fabs(max)){
      max = x[i*3 + (d-1)];
    }
  }
  
  return max;
}
