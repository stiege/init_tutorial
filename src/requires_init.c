#include "stdio.h"
#include "init_defs.h"

static void init1(void)
{
    printf("%s\n", "init1 called");
}

static void init2 (void)
{
    printf("%s\n", "init2 called");
}

REGISTER_INIT_FUNCTION(init1);
REGISTER_INIT_FUNCTION(init2);
