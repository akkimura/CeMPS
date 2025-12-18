#include "Ce_MPS.h"

void writeMesh(void){
  int i, j;
  FILE* fp;
  char fileName[600];

  snprintf(fileName,sizeof(fileName), "%s/membrane_%04d.vtk", mem_path, Num_of_File);
  fp = fopen(fileName, "w");

  /* write HEADER */
  fprintf(fp, "# vtk DataFile Version 3.0\n");
  fprintf(fp, "vtk output\n");
  fprintf(fp, "ASCII\n");
  fprintf(fp, "DATASET POLYDATA\n");
  fprintf(fp, "POINTS %d float\n", num_p);

  /* write Point Data */
  for(j=0; j<num_p; j++){
    if(pcl[j].isIgn == YES){
      fprintf(fp, "0 0 0\n");
    } else{
      i = pcl[j].num;
      fprintf(fp, "%e %e %e\n", Position[i*3+X], Position[i*3+Y], Position[i*3+Z]);
    }
  }

  fprintf(fp, "\nVERTICES %d %d\n", num_p, num_p*2);
  for(j=0; j<num_p; j++){
    fprintf(fp, "1 %d\n", j);
  }

  fprintf(fp, "\nPOLYGONS %d %d\n", num_t, num_t*4);
  for(j=0; j<num_t; j++){
    fprintf(fp, "3 %d %d %d\n", tri[j].p1, tri[j].p2, tri[j].p3);
  }

  fclose(fp);
}
