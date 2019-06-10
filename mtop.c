#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  /* Syscall invocation here */

  int returnvalue = memtop();
  printf(1, "available memory: %d", returnvalue);

  int pid = fork();
  if (pid == 0) {
    exit();
  }
  else {
  wait();

  int maxproc = pid;
  int NL = 512;
  char name[NL];
  /* printf(1, "\nMax proc id = %d", maxproc); */
  int i;
  for (i = 1; i < maxproc; i++){
    int bytes = getmeminfo(i, name, NL);
    if (bytes == -1) continue;
    printf(1, "\npid: %d, ", i);
    printf(1, "name: %s, ", name);
    printf(1, "mem: %d", bytes);
  }
  printf(1,"\n");
  exit();
}
}
