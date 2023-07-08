#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

string s_toupper (string s);
int main(int argc, string argv[])
{
//get key from command line argument

//ensure that key is provided
    if(argv[1] == NULL)
    {
        printf("Provide Key\n");
        return 1;
    }

 string key = s_toupper(argv[1]);
// ensure that there is only 1 key
    if(argc != 2)
    {
        printf("Only provide key\n");
        return 1;
    }
//ensure that there are 26 letters in the key
    if (!(strlen(key) == 26))
    {
        printf("Only have 26 letters in key\n");
        return 1;
    }

// ensure there are no repeat letters
    for(int i = 0; i < 26; i++)
    {
        for (int n = 1; (i + n) < 26; n++)
        {
            if (key[i] == key [i+n])
            {
                printf("No repeated letters in the key\n");
                return 1;
            }
        }
    }

//ensure that the key only has letters
    for(int i = 0; i < 26; i++)
    {
        if(!isalpha(key[i]))
        {
           printf("Key can only include letters\n");
           return 1;
        }
    }

// get plaintext from user
    string text = get_string("plaintext:");
    int n = strlen(text);

//cipher the code
    char cipher[n];
    for (int i = 0; i < n ; i++)
    {
        if(isalpha(text[i]))
        {
            if(isupper(text[i]))
            {
                cipher[i] = toupper(key[text[i] - 'A']);
            }
            else
            {
                cipher[i] = tolower(key[text[i] - 'a']);
            }
        }
        else
        cipher[i] = text[i];
    }
    cipher[n] = '\0';
    printf("ciphertext: %s\n", cipher);
    return 0;
}

string s_toupper (string s)
{
    for(int i = 0; i < 26 ; i++)
    {
        s[i] = toupper(s[i]);
    }
    return s;
}
