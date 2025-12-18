#include "Ce_MPS.h"

void freeVariables(void){
  int i;

  free(Acceleration);
  free(ParticleType);
  free(Position);
  free(Velocity);
  free(Viscosity_x);
  free(Viscosity_y);
  free(Viscosity_z);
  free(Pressure);
  free(NumberDensity);
  free(BoundaryCondition);
  free(SourceTerm);
  free(SourceTerm_vx);
  free(SourceTerm_vy);
  free(SourceTerm_vz);
  free(FlagForCheckingBoundaryCondition);
  free(CoefficientMatrix);
  free(MinimumPressure);
  free(isOutOfCell);
  for(i=0; i<6; i++){
    free(adjacentParticle[i]);
  }
  free(adjacentParticle);
  free(tri);
  free(pcl);
  for(i=0; i<num_t; i++){
    free(adjacentTriangle[i]);
  }
  free(adjacentTriangle);
}
