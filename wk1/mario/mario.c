#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h, r, c, d;
    do
    {
        h = get_int("Height: ");
    }
    while ( h < 1 || h > 8 );

    // for columns
    for ( c = 0; c < h; c++)
    {
        // for rows (dots)

        for ( d = h - 1 ; d > c ; d--)
          {
            printf(" ");
          }
        // for rows (#)
        for ( r = 0; r <= c; r++)
        {
          printf("#");
        }
        printf("\n");
    }
}
