#include "Ce_MPS.h"

void sendNotification(void){
  char SysCall[1000];

  sprintf(SysCall, "./mail_sender.sh");
  system(SysCall);
  sprintf(SysCall, "./line_sender.sh");
  system(SysCall);
}
