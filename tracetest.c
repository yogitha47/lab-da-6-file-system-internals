#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{

    printf(1, "Tracing is ON\n");

    trace(1);

    write(1, "Hello xv6\n", 10);

    trace(0);

    printf(1, "Tracing is OFF\n");

    exit();
}
