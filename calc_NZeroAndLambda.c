#include "Ce_MPS.h"

void calc_NZeroAndLambda(void){
  const int nX = (int)(L_AXIS / DISTANCE_OF_PARTICLES) + 30;
  const int nY = (int)(S_AXIS / DISTANCE_OF_PARTICLES) + 30;
  const int nZ = (int)(S_AXIS / DISTANCE_OF_PARTICLES) + 30;

  const int iZ_start = -nZ/2;
  const int iZ_end = nZ/2;

  N0_4NumberDensity = 0.0;
  N0_4Gradient      = 0.0;
  N0_4Laplacian     = 0.0;
  Lambda              = 0.0;
  const double xi = 0.0,  yi = 0.0,  zi = 0.0;

  for(int iX=-nX/2; iX<nX/2; iX++){
    for(int iY=-nY/2; iY<nY/2; iY++){
      for(int iZ=iZ_start; iZ<iZ_end; iZ++){
        if(((iX == 0) && (iY == 0)) && (iZ == 0)) continue;
        const double xj = DISTANCE_OF_PARTICLES * (double)(iX);
        const double yj = DISTANCE_OF_PARTICLES * (double)(iY);
        const double zj = DISTANCE_OF_PARTICLES * (double)(iZ);
        const double distance = sqrt(pow(xj-xi, 2) + pow(yj-yi, 2) + pow(zj-zi, 2));
        N0_4NumberDensity += weight(distance, Re_4NumberDensity);
        N0_4Gradient      += weight(distance, Re_4Gradient);
        N0_4Laplacian     += weight(distance, Re_4Laplacian);
        Lambda              += pow(distance, 2) * weight(distance, Re_4Laplacian);	
      }
    }
  }
  Lambda = Lambda / N0_4Laplacian;

  /* debug start */
  /*   double ra = DISTANCE_OF_PARTICLES / sqrt(M_PI); */
  /*   double rliml = 4.000001 * DISTANCE_OF_PARTICLES; */
  /*   Lambda = ( ( pow( rliml, 4 ) / 12.0 - ( rliml * pow( ra, 3 ) ) / 3.0 + pow( ra, 4 ) / 4.0 ) ) / ( ( pow( rliml, 2 ) / 2.0 - ( rliml * ra ) + pow( ra, 2 ) / 2.0 ) ); */
  /* debug end */

  printf("\nN0_4NumberDensity = %e\n", N0_4NumberDensity);
  printf("N0_4Gradient = %e\n", N0_4Gradient);
  printf("N0_4Laplacian = %e\n", N0_4Laplacian);
  printf("Lambda = %e\n\n", Lambda);
}
