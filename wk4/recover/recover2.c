#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>

typedef uint8_t BYTE;
int main(int argc, char *argv[])
{
    //alloc memory to a string
    string filename = malloc(sizeof(char) * 8); //to allocate memory to the filename

    // if malloc fail will return null pointer
    if (filename == NULL) //bc if malloc is successful itll return a void pointer
    {
        return 1;
    }

    int image_counter = 0;
    //This file ptr must be declared earlier as I mentioned output_file incase of error in front of the while loop
    FILE *output_file = NULL;
    const int BLOCKSIZE = 512; //number of bytes in a block
    // checking validity of input
    if (argc != 2)
    {
        printf("Invalid input\n");
        return 1;
    }
    // open the card
    FILE *input_file = fopen(argv[1], "r");
    if(input_file == NULL)
    {
        printf("Invalid file\n");
        return 1;
    }
    BYTE buffer[BLOCKSIZE];
    // repeat the card
    //working under the assumption that fread never fails
    while(fread(buffer, sizeof(BYTE), BLOCKSIZE, input_file)) //read into the buffer
    {
        //check if that block is a jpeg file
        if(buffer[0] == 0xff &&  buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if(image_counter >= 1)
            {
                fclose(output_file);
            }
            sprintf(filename, "%03i.jpg", image_counter);
            output_file = fopen(filename, "w");
            //Check if output file has been opened
            if(output_file == NULL)
            {
                free(filename);
                fclose(input_file);
                printf("Output file corrupted\n");
                return 0;
            }
            image_counter++;
        }
        //Else continue process current jpg if block is not start of another jpg
        if(image_counter >= 1)
        {
            int i = fwrite(buffer, sizeof(BYTE), BLOCKSIZE, output_file);
            //If fwrite fails
            if(i != sizeof(BYTE) * BLOCKSIZE)
            {
                free(filename);
                fclose(output_file);
                fclose(input_file);
                printf("fwrite failed\n");
                return 0;
            }
        }
    }
    //Close remaining files
    fclose(input_file);
    fclose(output_file);
    free(filename);
    return 0;
}