#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    if(argc > 1)
        printf(1, "Hello, %s!\n", argv[1]);
    else
        printf(1, "Hello, xv6 User!\n");

    exit();
}
