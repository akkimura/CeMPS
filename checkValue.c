#include "Ce_MPS.h"

void checkValue(double time, int step, char* func){
  int i;
  FILE* fp;
  
  #pragma omp parallel for schedule(dynamic,64)
  for(i=0; i< Num_of_Particles; i++){
    if(Position[i*3+X] > 1.0){
      fp = fopen("error_log.txt", "w");
      fprintf(fp, "bug occured(Pos[X] = %e) in %s function at %d step (t=%lf)\n", Position[i*3+X], func, step, time);
      fclose(fp);
      sendNotification();
      exit(1);
    }
  }
}
