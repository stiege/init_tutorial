#include "stdio.h"
#include "init_defs.h"

extern init_func ** __init_start;
extern init_func ** __init_end;

void main(void)
{
    init_func ** inits = __init_start;

    if (NULL != __init_start)
    {
        printf("%s\n", "Beginning Init");
        do
        {
            (*inits++)();
        }while (inits <= __init_end);
        printf("%s\n", "End of init");
    }
    else
    {
        printf("%s\n", "No init functions registered");
    }
}
