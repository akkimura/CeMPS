#ifndef _CE_MPS_H_
#define _CE_MPS_H_

#include <getopt.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

/* debug mode */
#define DEBUG OFF

/* output setting */
#define OUTPUT_INTERVAL 20
#define OUTPUT_CSV OFF
#define OUTPUT_MEM_CSV ON
#define DISTANCE_OF_PARTICLES 1.5E-6
// #define DISTANCE_OF_PARTICLES       3.0E-6

/* parameters for modeling */
#define L_AXIS 50E-6
#define S_AXIS 30E-6
#define RADIUS_FOR_CENTROSOME 7.0E-6

#define MAX_X L_AXIS / 2 + 8.0 * DISTANCE_OF_PARTICLES
#define MIN_X -MAX_X
#define MAX_Y S_AXIS / 2 + 8.0 * DISTANCE_OF_PARTICLES
#define MIN_Y -MAX_Y
#define MAX_Z S_AXIS / 2 + 8.0 * DISTANCE_OF_PARTICLES
#define MIN_Z -MAX_Z

#define SIZE_OF_ARRAY 50000
#define MAX 100
#define DT 0.1
#define SIMULATION_TIME 2.0

#define FLUID_DENSITY 1.0
#define MASS_DENSITY 1.0E3
#define KINEMATIC_VISCOSITY FLUID_DENSITY / MASS_DENSITY
#define SPRING_CONSTANT 1.0E-6
#define BENDING_CONSTANT 1.0E-12

#define G_X 0.0
#define G_Y 0.0
#define G_Z 0.0

/* parameters for MPS */
#define RADIUS_4_NUM_DENS (2.1 * DISTANCE_OF_PARTICLES)
#define RADIUS_4_GRAD (2.1 * DISTANCE_OF_PARTICLES)
#define RADIUS_4_LAPLACIAN (4.000001 * DISTANCE_OF_PARTICLES)
#define DISTANCE_OF_COLLISION (0.5 * DISTANCE_OF_PARTICLES)
#define THRESH_RATIO_OF_NUM_DENS 0.97
#define COEFF_OF_RESTITUTION 0.2
#define COMPRESSIBILITY 0.45E-9
#define EPS (0.01 * DISTANCE_OF_PARTICLES)
#define RELAX_COEFF_4_PRESSURE 0.2

/* etc */
#define GHOST -1
#define DUMMY_WALL 0
#define WALL 1
#define MEMBRANE 2
#define CENTROSOME 3
#define FLUID 4
#define FLUID_OUT 5
#define GHOST_OR_DUMMY -1
#define SURFACE_PARTICLE 1
#define INNER_PARTICLE 0
#define FLAG_DIRICHLET_BOUNDARY_IS_NOT_CONNECTED 0
#define FLAG_DIRICHLET_BOUNDARY_IS_CONNECTED 1
#define FLAG_DIRICHLET_BOUNDARY_IS_CHECKED 2

#define ON 1
#define OFF 0
#define YES 1
#define NO 0

#define X 0
#define Y 1
#define Z 2

#define NORMAL 0
#define SQUARE 1
#define RECTANGLE 2
#define TRIANGLE 3
#define CIRCLE 4
#define VSHAPE 5
#define VSHAPE_2 6

#define EX 1

/* struct for mesh making */
typedef struct {
  int num;
  double dist;
  double x;
  double y;
  double z;
  int isIgn;
} particle_t;

typedef struct {
  int p1;
  int p2;
  int p3;
} triangle_t;

/* functions */
void initialize_Position_and_Velocity(int shape);
void mainSimulation(void);
void calc_ConstantParam(void);
void calc_NZeroAndLambda(void);
void calc_Gravity(void);
void calc_Viscosity(void);
void calc_Pressure(void);
void calc_NumDens(void);
void calc_Spring(void);
void calc_Spring_for3dim(void);
void calc_PressureGrad(void);
void set_BoundaryCondition(void);
void set_SourceTerm(void);
void set_Matrix(void);
void set_SourceTerm_4Viscosity(void);
void set_Matrix_4Viscosity(void);
void set_Min_Pressure(void);
void remove_Neg_Pressure(void);
void searchAdjacentParticle(void);
void inputAdjacentParticle(char *filename);
void collision(void);
void excep_Process_4BoundaryCondition(void);
void verify_BoundaryCondition(void);
void extendDiagonalTerm(double *Matrix);

int solveSimultaniousEquationsByConjugateGradientMethod(double *A, double *b,
                                                        double *x, int n,
                                                        int max_iter,
                                                        double eps);

void moveParticle(double Time);
void moveParticle_with_PressureGrad(void);

void makeOutputDir(void);
void writeData(void);
void writeInitialData(void);
void writePointData(void);
void writeMembraneData(void);
void writeMesh(void);
void sendNotification(void);
void outputForGnuplot(void);
void writeSetting(unsigned long total);

void mallocVariables(void);
void freeVariables(void);

double calDotProduct(double *x, double *y, int n);
double calDistance(int i, int j);
double weight(double distance, double re);
double findMaxValue(double *x, int d);
int cmp(const void *p, const void *q); // for qsort
void checkValue(double time, int step, char *func);

// for Bucket&List
void initializeBucket(void);
void makeBucket(void);
void freeBucket(void);
// for 3DSpring
void replacePointNum(void);
void searchAdjacentTriangle(void);

/* global variables */
double *Acceleration;
int *ParticleType;
double *Position;
double *Velocity;
double *Viscosity_x;
double *Viscosity_y;
double *Viscosity_z;
double *Pressure;
double *NumberDensity;
int *BoundaryCondition;
double *SourceTerm;
double *SourceTerm_vx;
double *SourceTerm_vy;
double *SourceTerm_vz;
int *FlagForCheckingBoundaryCondition;
double *CoefficientMatrix;
double *MinimumPressure;
int *isOutOfCell;
int **adjacentParticle;
int Num_of_File;
double Time;
double dt;
int Num_of_Particles;
double Re_4NumberDensity, Re_sq_4NumberDensity;
double Re_4Gradient, Re_sq_4Gradient;
double Re_4Laplacian, Re_sq_4Laplacian;
double N0_4NumberDensity;
double N0_4Gradient;
double N0_4Laplacian;
double Lambda;
double Dis_of_collision, Dis_of_collision_sq;
double Fluid_Dens;
double asym;
double MaxWidth;
double Centro_L, Centro_R;
double meanWallDistance;
double threshold;
char dir_path[256], data_path[512], wall_path[512], mem_path[512];
// for Bucket&List
double BucketSize;
int nBx, nBy, nBz, nBxy, nBxyz;
int *bfst, *blst, *nxt;
// for 3DSpring
int num_p, num_t;
particle_t *pcl;
triangle_t *tri;
int **adjacentTriangle;

#endif /* _CE_MPS_H_ */
