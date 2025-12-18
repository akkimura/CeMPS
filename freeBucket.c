#include "Ce_MPS.h"

void freeBucket(void){
  free(bfst);
  free(blst);
  free(nxt);
}

