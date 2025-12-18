#include "Ce_MPS.h"

double weight(double distance, double re){
  double weightIJ;

  if(distance >= re){
    weightIJ = 0.0;
  } else{
    weightIJ = pow(1 - distance/re , 3) * pow(1 + distance/re, 3);
  }
  return weightIJ;
}
