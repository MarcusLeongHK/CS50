#include <cs50.h>
#include <stdio.h>

int collatz(void)
{
    int i = get_int("Number: ");
    printf("Steps: %i\n", n);

    if (i == 1)
    {
        return 0;
    }
    else if (i%2 == 0)
    {
       return 1 + collatz(i/2);
    }
    else
    {
       return 1 + collatz( 3*i + 1);
    }
}