// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

//declare book file pointer globally
FILE *book = NULL;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//Counter for number of words
int counter = 0;

// TODO: Choose number of buckets in hash table
const unsigned int N = 53;
int location = 0;

// Hash table
node *table[N] = {NULL}; //this is a pointer to a node it is not a node itself

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    location = hash(word);
    node *cursor = table[location];
    if(cursor == NULL)
    {
        return false;
    }
    while(cursor != NULL)
    {
        if(strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int sum = 0;
    // TODO: Improve this hash function
    for(int i = 0, n = strlen(word); i < n ; i++)
    {
        sum += toupper(word[i]);
    }
    if((sum % N) > (N - 1))
    {
        return (sum % N) % N;
    }
    else
    {
        return sum % N;
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    char dictwords[46];
    //Open dictionary
    book = fopen(dictionary, "r");
    if(book == NULL)
    {
        printf("Unable to open file\n");
        return false;
    }
    //Loop through each word in dictionary
    while(fscanf(book, "%s", dictwords) != EOF)
    {
        //Create a new node for each word
        node *n = malloc(sizeof(node));
        if(n == NULL)
        {
            unload();
            return false;
        }
        strcpy(n->word, dictwords);
        //Hash word to obtain index(location)
        location = hash(dictwords);
        //Insert word into hash table at that index(location)
        if(table[location] == NULL) //first linked list at that index(location)
        {
            table[location] = n;
            n->next = NULL;
            counter++;
        }
        else
        {
            n->next = table[location];
            table[location] = n;
            counter++;
        }
    }
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for(int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while(cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    fclose(book);
    return true;
}

