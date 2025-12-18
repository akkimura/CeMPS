#include "Ce_MPS.h"

void writeSetting(unsigned long total){
  FILE* fp;
  time_t timer;
  int hour, minute, second;
  char fileName[512];

  /* get time */
  time(&timer);

  snprintf(fileName,sizeof(fileName), "%s/SETTING", dir_path);
  fp = fopen(fileName, "w");

  fprintf(fp, "///// result of the simulation /////\n\n");
  
  fprintf(fp, "Date : %s\n\n", ctime(&timer));
  
  fprintf(fp, "Particle Distance [m]     : %e\n", DISTANCE_OF_PARTICLES);
  fprintf(fp, "dt [s]                    : %e\n", dt);
  fprintf(fp, "Output Interval [step]    : %d\n", OUTPUT_INTERVAL);
  fprintf(fp, "Finish Time [s]           : %lf\n\n", SIMULATION_TIME);
  fprintf(fp, "Radius for Centrosome [m] : %e\n\n", RADIUS_FOR_CENTROSOME);
  
  fprintf(fp, "Kinematic Viscosity(nu) [m^2/s] : %e\n", KINEMATIC_VISCOSITY);
  fprintf(fp, "Fluid Density(rho) [kg/(m s)]   : %e\n\n", FLUID_DENSITY);
  
  fprintf(fp, "X-Force [m/(s^2)] : %e\n", G_X);
  fprintf(fp, "Y-Force [m/(s^2)] : %e\n", G_Y);
  fprintf(fp, "Z-Force [m/(s^2)] : %e\n\n", G_Z);
  
  fprintf(fp, "Radius for Number Density [m]       : %lf * DISTANCE_OF_PARTICLES\n", RADIUS_4_NUM_DENS / DISTANCE_OF_PARTICLES);
  fprintf(fp, "Radius for Gradient [m]             : %lf * DISTANCE_OF_PARTICLES\n", RADIUS_4_GRAD / DISTANCE_OF_PARTICLES);
  fprintf(fp, "Radius for Laplacian [m]            : %lf * DISTANCE_OF_PARTICLES\n", RADIUS_4_LAPLACIAN / DISTANCE_OF_PARTICLES);
  fprintf(fp, "Collision Distance [m]              : %lf * DISTANCE_OF_PARTICLES\n", DISTANCE_OF_COLLISION / DISTANCE_OF_PARTICLES);
  fprintf(fp, "Threshold Ration of Number Density  : %e\n", THRESH_RATIO_OF_NUM_DENS);
  fprintf(fp, "Coefficient of Restitution          : %e\n", COEFF_OF_RESTITUTION);
  fprintf(fp, "Compressibility [1/Pa]              : %e\n", COMPRESSIBILITY);
  fprintf(fp, "EPS [m]                             : %lf * DISTANCE_OF_PARTICLES\n", EPS / DISTANCE_OF_PARTICLES);
  fprintf(fp, "Relaxation Coefficient For Pressure : %e\n\n", RELAX_COEFF_4_PRESSURE);

  fprintf(fp, "N0_4NumberDensity : %e\n", N0_4NumberDensity);
  fprintf(fp, "N0_4Gradient      : %e\n", N0_4Gradient);
  fprintf(fp, "N0_4Laplacian     : %e\n\n", N0_4Laplacian);

  fprintf(fp, "Lambda : %e\n\n", Lambda);
  
  fprintf(fp, "Total Simulation Time [s] : %lu\n", total);

  hour   = (int)(total/3600);
  minute = ((int)(total) - hour * 3600) / 60;
  second = (int)(total) % 60;
  fprintf(fp, "Total Simulation Time [h:m:s] : %d:%d:%d", hour, minute, second);
  
  fclose(fp);
}
