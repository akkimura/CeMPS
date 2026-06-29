#include "Ce_MPS.h"

void mainSimulation(void){
  int i, iTimeStep = 0;

  if(OUTPUT_CSV == ON){
    writePointData();
  }
  if(OUTPUT_MEM_CSV == ON){
    writeMembraneData();
  }
  writeData();

  dt = DT;

  for(i=0; i<Num_of_Particles; i++){
    Viscosity_x[i] = 1e-15;
    Viscosity_y[i] = 1e-15;
    Viscosity_z[i] = 1e-15;
    Pressure[i]    = 1e-15;
  }

  while(1){
    makeBucket();
    calc_Gravity();
    printf("Start calc_Spring\n");
    calc_Spring_for3dim();
    printf("Start calc_Viscosity\n");
    calc_Viscosity();
    calc_TruePressure();
    moveParticle(Time);
    checkValue(Time, iTimeStep, "moveParticle");
    collision();
    printf("Start calc_Pressure\n");
    calc_Pressure();
    calc_PressureGrad();
    moveParticle_with_PressureGrad();

    printf("iTimeStep++ (iTimeStep = %d)\n", iTimeStep+1);
    iTimeStep++;
    Time += dt;

    if((iTimeStep % OUTPUT_INTERVAL) == 0){
      printf("TimeStepNumber: %4d   Time: %lf(s)   Num_of_Particles: %d\n", iTimeStep, Time, Num_of_Particles);
      if(OUTPUT_CSV == ON){
        writePointData();
      }
      if(OUTPUT_MEM_CSV == ON){
        writeMembraneData();
      }
      writeData();
    }
    if(Time >= SIMULATION_TIME){
      break;
    }
  }
}
