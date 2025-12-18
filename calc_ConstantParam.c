#include "Ce_MPS.h"

void calc_ConstantParam(void){

  Re_4NumberDensity    = RADIUS_4_NUM_DENS;  
  Re_4Gradient         = RADIUS_4_GRAD;  
  Re_4Laplacian        = RADIUS_4_LAPLACIAN;  
  Re_sq_4NumberDensity = Re_4NumberDensity * Re_4NumberDensity;
  Re_sq_4Gradient      = Re_4Gradient * Re_4Gradient;
  Re_sq_4Laplacian     = Re_4Laplacian * Re_4Laplacian;
  calc_NZeroAndLambda();
  Fluid_Dens           = FLUID_DENSITY;
  Dis_of_collision     = DISTANCE_OF_COLLISION; 
  Dis_of_collision_sq  = Dis_of_collision * Dis_of_collision;
  Num_of_File          = 0;
  Time                 = 0.0;
}
