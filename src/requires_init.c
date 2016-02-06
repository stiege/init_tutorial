#include "stdio.h"
#include "init_defs.h"

void init1(void)
{
    printf("%s\n", "init1 called");
}

void init2 (void)
{
    printf("%s\n", "init2 called");
}

__attribute__((section(".init_functions")))
init_func * init_array[] = {
    &init1,
    &init2
};
