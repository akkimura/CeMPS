/*
 * Author: Yusuke Yoshimi <yoshimi@fun.bio.keio.ac.jp>
 * Last modified: Fri, 08 Dec 2017 16:17:12 +0900
 */
#include "Ce_MPS.h"

void makeOutputDir(void){
  time_t t;
  char date[64];  
  
  /* make result dir */
  mkdir("result", 0755);
  
  /* output dir. setting */
  t = time(NULL);
  strftime(date, sizeof(date), "%Y%m%d_%H%M%S", localtime(&t));
  snprintf(dir_path, sizeof(dir_path), "result/%s", date);
  mkdir(dir_path, 0755);

  snprintf(data_path, sizeof(data_path), "%s/data", dir_path);
  mkdir(data_path, 0755);

  snprintf(wall_path, sizeof(wall_path), "%s/wall", dir_path);
  mkdir(wall_path, 0755);

  snprintf(mem_path, sizeof(mem_path), "%s/membrane", dir_path);
  mkdir(mem_path, 0755);
}
