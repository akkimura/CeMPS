#include "Ce_MPS.h"

void remove_Neg_Pressure(void){
  int i;

  #pragma omp parallel for
  for(i=0; i< Num_of_Particles; i++){
    if(isOutOfCell[i] == YES){
      if(Pressure[i] < 0.0){
        Pressure[i] = 0.0;
      }
    }
  }
}
