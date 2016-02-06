#include "stdio.h"
#include "init_defs.h"

void init1(void)
{
    printf("%s\n", "init1 called");
}

void init2(void)
{
    printf("%s\n", "init2 called");
}

init_func * init_array[] = {
    &init1,
    &init2
};

init_func ** __init_start = &init_array[0];
init_func ** __init_end = &init_array[1];
