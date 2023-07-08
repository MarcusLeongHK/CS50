#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string para);
int count_words(string para);
int count_sentences(string para);

int main(void)
{
    string text = get_string("Text: \n");
    float nletters = count_letters(text);
    float nwords = count_words(text);
    float nsentences = count_sentences(text);

    float L = (100 / nwords) * nletters;
    float S = (100 /  nwords) * nsentences;
    float index = 0.0588 * L - 0.296 * S - 15.8;

    if(index > 16)
    {
        printf("Grade 16+\n");
    }
    else if(index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", (int)round(index));
    }

}

int count_letters(string para)
{
    int letters = 0;
    for( int i = 0, n = strlen(para); i < n; i++)
    {
        if(isalpha(para[i]))
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string para)
{
    int words = 0;
    for( int i = 0, n = strlen(para); i < n; i++)
    {
        if(isspace(para[i]))
        {
            words++;
        }
    }
    return words + 1;
}

int count_sentences(string para)
{
    int sentences = 0;
    for( int i = 0, n = strlen(para); i < n; i++)
    {
        if(para[i] == '.' || para[i] == '!' || para[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}