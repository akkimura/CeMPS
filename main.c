#include "Ce_MPS.h"

int main(int argc, char* argv[]){
  int result = 0, shape = NORMAL;

  asym  = OFF;

  while((result = getopt(argc, argv, "hs:y")) != -1){
    switch(result){
      case 'h': // help
        printf("Usage: By adding options below, you can change the shape and the color of cell.\n");
        printf("       -s [shape] : (shape = normal, square, rectangle, triangle, circle, vshape, lshape)\n");
        printf("                    (default: normal)\n");
        printf("       -y: asymmetry division (shape is Normal when asymmetry)\n");
        exit(0);
      case 's': // shape
        if(strcmp(optarg, "normal") == 0){
          shape = NORMAL;
        } else if(strcmp(optarg, "square") == 0){
          shape = SQUARE;
        } else if(strcmp(optarg, "rectangle") == 0){
          shape = RECTANGLE;
        } else if(strcmp(optarg, "triangle") == 0){
          shape = TRIANGLE;
        } else if(strcmp(optarg, "circle") == 0){
          shape = CIRCLE;
        } else if(strcmp(optarg, "vshape") == 0){
          shape = VSHAPE;
        } else if(strcmp(optarg, "lshape") == 0){
          shape = VSHAPE_2;
        } else{
          printf("Incorrect shape specification (shape = normal, square, rectangle, triangle, circle, vshape, lshape)\n");
          exit(1);
        }
        break;
      case 'y': // asymmetry division
        asym = ON;
        shape = NORMAL;
        break;
      default:  // other case
        printf("Usage: By adding options below, you can change the shape and the color of cell.\n");
        printf("       -s [shape] : (shape = normal, square, rectangle, triangle, circle, vshape, lshape)\n");
        printf("                    (default: normal)\n");
        printf("       -y: asymmetry division (shape is Normal when asymmetry)\n");
        exit(1);
    }
  }

  printf("\n*** START PARTICLE-SIMULATION! ***\n");
  makeOutputDir();

  mallocVariables();

  initialize_Position_and_Velocity(shape);

  printf("Finish initializing : Num of Particles = %d\n", Num_of_Particles);

  initializeBucket();

  writeInitialData();

  calc_ConstantParam();
  
  if(DEBUG != ON){
    inputAdjacentParticle(argv[argc-1]);
    replacePointNum();
    searchAdjacentTriangle();
    // outputForGnuplot();
  }

  printf("Loop START!!!\n");
  mainSimulation();
  printf("Loop END!!!\n");

  if(OUTPUT_CSV == ON){
    writePointData();
  }
  if(OUTPUT_MEM_CSV == ON){
    writeMembraneData();
  }
  writeData();

  writeSetting(0);

  printf("*** END ***\n");

  sendNotification();

  freeVariables();
  freeBucket();

  return 0;
}
