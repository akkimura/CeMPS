#include "Ce_MPS.h"

double calDistance(int i, int j){
  double dist = 0;

  dist = sqrt(pow(Position[j*3+X] - Position[i*3+X], 2) + pow(Position[j*3+Y] - Position[i*3+Y], 2) + pow(Position[j*3+Z] - Position[i*3+Z], 2));

  return dist;
}
