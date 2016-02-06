#include "stdio.h"
typedef void (init_func)(void);

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

void main(void)
{
    printf("%s\n", "Beginning Init");
    init_func ** inits = __init_start;

    if (NULL != __init_start)
    {
        do
        {
            (*inits)();
            inits++;
        }while (inits <= __init_end);
    }
    printf("%s\n", "End of init");
}
