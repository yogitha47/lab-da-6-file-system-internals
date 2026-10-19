#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int i;
  int pid;
  int n = 1000;

  shared_reset();

  for(i = 0; i < 4; i++){
    pid = fork();

    if(pid == 0){
      for(i = 0; i < n; i++)
        shared_inc();

      exit();
    }
  }

  for(i = 0; i < 4; i++)
    wait();

  printf(1, "Expected counter = %d\n", 4 * n);
  printf(1, "Actual counter   = %d\n", shared_get());

  exit();
}
