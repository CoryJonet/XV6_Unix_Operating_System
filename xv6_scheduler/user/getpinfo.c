#include "types.h"
#include "stat.h"
#include "pstat.h"
#include "user.h"

int getprocinfo(void) {
  
  struct pstat p;

  int i;
  for (i = 0; i < 64; i++) {

    p.inuse[i] = 0;
    p.pid[i] = -1;
    p.hticks[i] = 0;
    p.lticks[i] = 0;

  }

  int temp = getpinfo(&p);

  return temp;

}
int
main(void) {

  //  getprocinfo();
  exit();

}
