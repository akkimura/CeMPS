#include "Ce_MPS.h"

void writeData(void){
  int idx;
  double absoluteValueOfVelocity;
  FILE *fp;
  char fileName[1024];

  snprintf(fileName,sizeof(fileName), "%s/particle_%04d.vtu", dir_path, Num_of_File);
  fp = fopen(fileName, "w");
  fprintf(fp, "<?xml version='1.0' encoding='UTF-8'?>\n");
  fprintf(fp, "<VTKFile xmlns='VTK' byte_order='LittleEndian' version='0.1' type='UnstructuredGrid'>\n");
  fprintf(fp, "<UnstructuredGrid>\n");
  fprintf(fp, "<Piece NumberOfCells='%d' NumberOfPoints='%d'>\n", Num_of_Particles, Num_of_Particles);

  fprintf(fp, "<Points>\n");
  fprintf(fp, "<DataArray NumberOfComponents='3' type='Float32' Name='Position' format='ascii'>\n");
  for(int i=0; i<Num_of_Particles; i++){
    idx = i*3;
    fprintf(fp, "%.8lf %.8lf %.8lf\n", Position[idx], Position[idx + 1], Position[idx + 2]);
  }
  fprintf(fp, "</DataArray>\n");
  fprintf(fp, "</Points>\n");

  fprintf(fp, "<PointData>\n");
  fprintf(fp, "<DataArray NumberOfComponents='1' type='Int32' Name='ParticleType' format='ascii'>\n");
  for(int i=0; i<Num_of_Particles; i++){
    fprintf(fp, "%d\n", ParticleType[i]);
  }
  fprintf(fp,"</DataArray>\n");

  fprintf(fp, "<DataArray NumberOfComponents='1' type='Float32' Name='Velocity' format='ascii'>\n");
  for(int i=0; i<Num_of_Particles; i++){
    idx = i*3;
    absoluteValueOfVelocity=
      sqrt(Velocity[idx]*Velocity[idx] + Velocity[idx+1]*Velocity[idx+1] + Velocity[idx+2]*Velocity[idx+2]);
    fprintf(fp,"%.11lf\n", absoluteValueOfVelocity);
  }
  fprintf(fp, "</DataArray>\n");

  fprintf(fp, "<DataArray NumberOfComponents='1' type='Float32' Name='pressure' format='ascii'>\n");
  for(int i=0; i<Num_of_Particles; i++){
    fprintf(fp, "%.15lf\n", Pressure[i]);
  }
  fprintf(fp,"</DataArray>\n");

  fprintf(fp, "<DataArray NumberOfComponents='1' type='Float32' Name='true_pressure' format='ascii'>\n");
  for(int i=0; i<Num_of_Particles; i++){
    fprintf(fp, "%.15lf\n", TruePressure[i]);
  }
  fprintf(fp,"</DataArray>\n");

  fprintf(fp,"</PointData>\n");

  fprintf(fp,"<Cells>\n");
  fprintf(fp,"<DataArray type='Int32' Name='connectivity' format='ascii'>\n");
  for(int i=0; i<Num_of_Particles; i++){
    fprintf(fp, "%d\n", i);
  }
  fprintf(fp, "</DataArray>\n");
  fprintf(fp, "<DataArray type='Int32' Name='offsets' format='ascii'>\n");
  for(int i=0; i<Num_of_Particles; i++){
    fprintf(fp, "%d\n", i+1);
  }
  fprintf(fp,"</DataArray>\n");
  fprintf(fp,"<DataArray type='UInt8' Name='types' format='ascii'>\n");
  for(int i=0; i<Num_of_Particles; i++){
    fprintf(fp, "1\n");
  }
  fprintf(fp, "</DataArray>\n");
  fprintf(fp, "</Cells>\n");
  fprintf(fp, "</Piece>\n");
  fprintf(fp, "</UnstructuredGrid>\n");
  fprintf(fp, "</VTKFile>\n");
  fclose(fp);

  Num_of_File++;
}
