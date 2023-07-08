#include <cs50.h>
#include <stdio.h>
// logic is wrong 
int main(void)
{
    //get credit card number
    long credit = get_long("Credit Card Number: ");

    //get the identity of the card
    if(credit < 9999999999999)
    {
        if (credit / 1000000000000 >= 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("invalid\n");
        }
    }
    else if (credit < 999999999999999 )
    {
        if ((credit / 10000000000000 >= 34) || (credit / 10000000000000 >= 37) )
        {
            printf("AMEX\n");
        }
        else
        {
            printf("invalid\n");
        }
    }
    // check for 16 digit
    else if (credit < 9999999999999999 )
    {
        if ((credit / 100000000000000 >= 51) || (credit / 100000000000000 >= 52) ||(credit / 100000000000000 >= 53) ||(credit / 100000000000000 >= 54) ||(credit / 100000000000000 >= 55) )
        {
            printf("MASTERCARD\n");
        }
        else if (credit / 1000000000000000 >= 4)
        {
            printf("VISA\n");
        }
        else
        {
        printf("invalid\n");
        }

    }
    else
    {
        printf("invalid\n");
    }
}