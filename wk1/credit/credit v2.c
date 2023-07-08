#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long credit = get_long("Credit Card Number: ");

    int c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c1a,c2a,c3a,c4a,c5a,c6a,c7a,c8a;

//finding the value of the alternating numbers starting from the second last digit
    c1 = ((credit % 100) / 10) * 2;
    c2 = ((credit % 10000) / 1000) * 2;
    c3 = ((credit % 1000000) / 100000) *2;
    c4 = ((credit % 100000000) / 10000000) * 2;
    c5 = ((credit % 10000000000) /1000000000) * 2;
    c6 = ((credit % 1000000000000) / 100000000000) * 2;
    c7 = ((credit % 100000000000000) / 10000000000000) * 2;
    c8 = ((credit % 10000000000000000) / 1000000000000000) *2;

//splitting the value of all possible 2 digit integers into a sum of both digits
    c1a = (c1 % 100/10) + (c1%10);
    c2a = (c2 % 100/10) + (c2%10);
    c3a = (c3 % 100/10) + (c3%10);
    c4a = (c4 % 100/10) + (c4%10);
    c5a = (c5 % 100/10) + (c5%10);
    c6a = (c6 % 100/10) + (c6%10);
    c7a = (c7 % 100/10) + (c7%10);
    c8a = (c8 % 100/10) + (c8%10);

    int sum_1 = c1a+c2a+c3a+c4a+c5a+c6a+c7a+c8a;
// finding the value of the other set of alternating numbers & finding sum 2
    c9 =  ((credit % 10));
    c10 = ((credit % 1000) / 100);
    c11 = ((credit % 100000) / 10000);
    c12 = ((credit % 10000000) / 1000000);
    c13 = ((credit % 1000000000) /100000000);
    c14=  ((credit % 100000000000) / 10000000000);
    c15 = ((credit % 10000000000000) / 1000000000000);
    c16 = ((credit % 1000000000000000) / 100000000000000);

    int sum_2 = c9+c10+c11+c12+c13+c14+c15+c16;

// finding the final sum
    int final_sum = sum_1 + sum_2;

//identifying the validity of the card and the type of card
    if ((final_sum % 10) == 0) //check validity
    {
        if (credit/1000000000000 < 1)
        {
            printf("INVALID\n");
        }
        else if ((c8==0 && c16==0) || c8==8)
        {
            printf("VISA\n");
        }
        else if ( ((c7+c16== 11) || (c7+c16== 17)) && c8==0 )
        {
            printf("AMEX\n");
        }
        else if ( c8!=0 && ((c8+c16 >= 11) && (c8+c16 <= 16)))
        {
            printf("MASTERCARD\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

}