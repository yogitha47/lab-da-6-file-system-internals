#include "types.h"
#include "stat.h"
#include "user.h"

int global_var = 100;

int
main(void)
{
  int local_var = 200;
  int *heap_var;
  int pid;

  heap_var = malloc(sizeof(int));
  if(heap_var == 0){
    printf(1, "malloc failed\n");
    exit();
  }

  *heap_var = 300;

  printf(1, "\n--- Parent: initial mappings ---\n");
  printf(1, "Global address: %p value=%d\n",
         &global_var, global_var);
  vmdebug((int)&global_var);

  printf(1, "Local address: %p value=%d\n",
         &local_var, local_var);
  vmdebug((int)&local_var);

  printf(1, "Heap address: %p value=%d\n",
         heap_var, *heap_var);
  vmdebug((int)heap_var);

  pid = fork();

  if(pid < 0){
    printf(1, "fork failed\n");
    free(heap_var);
    exit();
  }

  if(pid == 0){
    global_var = 111;
    local_var = 222;
    *heap_var = 333;

    printf(1, "\n--- Child: modified values ---\n");
    printf(1, "Global=%d Local=%d Heap=%d\n",
           global_var, local_var, *heap_var);

    printf(1, "--- Child mappings ---\n");
    vmdebug((int)&global_var);
    vmdebug((int)&local_var);
    vmdebug((int)heap_var);

    free(heap_var);
    exit();
  }

  wait();

  printf(1, "\n--- Parent: values after child exits ---\n");
  printf(1, "Global=%d Local=%d Heap=%d\n",
         global_var, local_var, *heap_var);

  free(heap_var);
  exit();
}
