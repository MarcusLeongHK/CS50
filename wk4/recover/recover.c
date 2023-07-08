#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>

typedef uint8_t BYTE;
int main(int argc, char *argv[])
{
    //alloc memory to a string
    string filename = malloc(sizeof(char) * 8); //to allocate memory to the filename;

    // if malloc fail will return null pointer
    if (filename == NULL) //bc if malloc is successful itll return a void pointer
    {
        return 1;
    }

    FILE *output_file = NULL;
    int image_counter = 0;
    const int BLOCKSIZE = 512; //number of bytes in a block
    // checking validity of input
    if(argc != 2)
    {
        printf("Invalid input\n");
        return 1;
    }
    // open the card
    FILE *file = fopen(argv[1], "r");
    if(file == NULL)
    {
        printf("Invalid file\n");
        return 1;
    }
    BYTE buffer[512];
    // repeat the card
    while(fread(buffer, sizeof(BYTE), BLOCKSIZE, file)) //read into the buffer
    {
        //check if that block is a jpeg file
        if(buffer[0] == 0xff &&  buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            sprintf(filename, "%03i.jpg", image_counter);
            output_file = fopen(filename, "w");
            image_counter++;
        }
        //Else continue process current jpg if block is not start of another jpg
        if (output_file != NULL)
        {
            fwrite(buffer, sizeof(BYTE), BLOCKSIZE, output_file);
        }
    }
    //Close remaining files
    free(filename);
    fclose(output_file);
    fclose(file);
}