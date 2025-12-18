#include "Ce_MPS.h"

void mallocVariables(void){
  int i;

  Acceleration = (double*)malloc(sizeof(double) * 3 * SIZE_OF_ARRAY);
  ParticleType = (int*)malloc(sizeof(int) * SIZE_OF_ARRAY);
  Position = (double*)malloc(sizeof(double) * 3 * SIZE_OF_ARRAY);
  Velocity = (double*)malloc(sizeof(double) * 3 * SIZE_OF_ARRAY);
  Viscosity_x = (double*)malloc(sizeof(double) * SIZE_OF_ARRAY);
  Viscosity_y = (double*)malloc(sizeof(double) * SIZE_OF_ARRAY);
  Viscosity_z = (double*)malloc(sizeof(double) * SIZE_OF_ARRAY);
  Pressure = (double*)malloc(sizeof(double) * SIZE_OF_ARRAY);
  NumberDensity = (double*)malloc(sizeof(double) * SIZE_OF_ARRAY);
  BoundaryCondition = (int*)malloc(sizeof(int) * SIZE_OF_ARRAY);
  SourceTerm = (double*)malloc(sizeof(double) * SIZE_OF_ARRAY);
  SourceTerm_vx = (double*)malloc(sizeof(double) * SIZE_OF_ARRAY);
  SourceTerm_vy = (double*)malloc(sizeof(double) * SIZE_OF_ARRAY);
  SourceTerm_vz = (double*)malloc(sizeof(double) * SIZE_OF_ARRAY);
  FlagForCheckingBoundaryCondition = (int*)malloc(sizeof(int) * SIZE_OF_ARRAY);
  CoefficientMatrix = (double*)malloc(sizeof(double) * SIZE_OF_ARRAY * SIZE_OF_ARRAY);
  MinimumPressure = (double*)malloc(sizeof(double) * SIZE_OF_ARRAY);
  isOutOfCell = (int*)malloc(sizeof(int) * SIZE_OF_ARRAY);
  adjacentParticle = (int**)malloc(sizeof(int) * SIZE_OF_ARRAY * MAX);
  for(i=0; i<SIZE_OF_ARRAY; i++){
    adjacentParticle[i] = (int*)malloc(sizeof(int) * MAX);
  }

  // initialize
  for(i=0; i<SIZE_OF_ARRAY; i++){
    ParticleType[i] = GHOST;
    Pressure[i] = 0.0;
  }
}
