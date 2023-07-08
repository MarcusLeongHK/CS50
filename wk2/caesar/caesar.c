#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char cipher;
char rotate(char c, int key);
int main(int argc, string argv[])
{
    //get key from command line argument
    //ensure that there are only 2 strings in the array
    if(argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // ensure that the second word is only a digit
    string s = argv[1];
    for(int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if(!isdigit(s[i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    //get plaintext from user
    string text = get_string("plaintext:\n");

    //convert the key into an int
    int key = atoi(argv[1]);

    //encipher the code, convert the alphabetical array to ASCII ; print the ciphered text
    printf("ciphertext: ");
    for(int i = 0, n = strlen(text); i < n; i++)
    {
        char encrypted = rotate(text[i], key);
        printf("%c", encrypted);
    }
    printf("\n"); // to start a new line
}

char rotate(char c, int key) // cipher the char based on the key
{
    if(isalpha(c)) //if char is alphabetical
    {
        if(islower(c)) //if char is lowercase
        {
            cipher = ( c  + key - 'a') % 26 + 'a';
        }
        else if (isupper(c)) //if char is uppercase
        {
            cipher = ( c + key - 'A') % 26 + 'A';
        }
    }
    else //if char is punctuation or space
    {
        return c; //return char unciphered
    }
    return cipher; //return ciphered char to function
}
