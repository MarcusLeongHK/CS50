#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // this is for every row
    for(int i = 0; i < height; i++)
    {
        // this is for every column within each row
        for (int j = 0; j < width; j++)
        {
            int ave_rgb = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0 );
            image[i][j].rgbtRed = ave_rgb;
            image[i][j].rgbtGreen = ave_rgb;
            image[i][j].rgbtBlue = ave_rgb;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height ; i++)
    {
         for (int j = 0; j < width/2 ; j++)
         {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - (j+1)];
            image[i][width - (j+1)] = temp;
         }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // to store pixels temporarily
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0 ; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float counter = 0.00; // counting the number of valid surrounding pixels
            int totalr , totalg, totalb;
            totalr = totalg = totalb = 0;
            // account for surrounding pixels
            for(int x = - 1 ; x < 2; x++)
                {
                    for(int y = - 1; y < 2; y++)
                    {
                        int currentX = x + i;
                        int currentY = y + j;
                        //check if neighbouring pixel is valid
                        if(currentX < 0 || currentX > (height - 1)|| currentY < 0 || currentY > (width -1) )
                            {
                                continue;
                            }
                            else
                            {
                                counter++;
                                totalr += temp[currentX][currentY].rgbtRed;
                                totalg += temp[currentX][currentY].rgbtGreen;
                                totalb += temp[currentX][currentY].rgbtBlue;
                            }
                    }
                }
                image[i][j].rgbtRed = round(totalr / counter);
                image[i][j].rgbtGreen = round(totalg / counter);
                image[i][j].rgbtBlue = round(totalb / counter);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for(int i = 0; i < height; i++)
    {
        for( int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for(int j = 0 ; j < width; j++)
        {
            int Gxred, Gxgreen, Gxblue, Gyred, Gygreen, Gyblue;
            Gxred = Gxgreen = Gxblue = Gyred = Gygreen = Gyblue = 0;
            int counter = 1;
            //first do Gx
            //this is to look at the 9 surrounding pixels
            for(int x = -1; x < 2; x++)
            {
                for(int y = -1; y < 2; y++)
                {
                    int currentX = i + x;
                    int currentY = j + y;
                    //if the pixel is invalid then skip it since it is treated as value 0
                    if (currentX < 0 || currentX >= height || currentY < 0 || currentY >= width)
                    {
                        counter++;
                        continue;
                    }
                    else if(counter == 1 || counter == 7)
                    {
                        counter++;
                        Gxred += temp[currentX][currentY].rgbtRed * -1;
                        Gxgreen += temp[currentX][currentY].rgbtGreen * -1;
                        Gxblue += temp[currentX][currentY].rgbtBlue * -1;
                    }
                    else if(counter == 4)
                    {
                        counter++;
                        Gxred += temp[currentX][currentY].rgbtRed * -2;
                        Gxgreen += temp[currentX][currentY].rgbtGreen * -2;
                        Gxblue += temp[currentX][currentY].rgbtBlue * -2;
                    }
                    else if(counter == 2 || counter == 5 || counter == 8)
                    {
                        counter++;
                        continue;
                    }
                    else if(counter == 3 || counter == 9)
                    {
                        counter++;
                        Gxred += temp[currentX][currentY].rgbtRed * 1;
                        Gxgreen += temp[currentX][currentY].rgbtGreen * 1;
                        Gxblue += temp[currentX][currentY].rgbtBlue * 1;
                    }
                    else if(counter == 6)
                    {
                        counter++;
                        Gxred += temp[currentX][currentY].rgbtRed * 2;
                        Gxgreen += temp[currentX][currentY].rgbtGreen * 2;
                        Gxblue += temp[currentX][currentY].rgbtBlue * 2;
                    }
                }
            }
            //then do Gy
            counter = 1;
            for(int x = -1; x < 2; x++)
            {
                for(int y = -1; y < 2; y++)
                {
                    int currentX = i + x;
                    int currentY = j + y;
                    //if the pixel is invalid then skip it since it is treated as value 0
                    if (currentX < 0 || currentX >= height || currentY < 0 || currentY >= width)
                    {
                        counter++;
                        continue;
                    }
                    else if (counter == 1 || counter == 3)
                    {
                        counter++;
                        Gyred += temp[currentX][currentY].rgbtRed * -1;
                        Gygreen += temp[currentX][currentY].rgbtGreen * -1;
                        Gyblue += temp[currentX][currentY].rgbtBlue * -1;
                    }
                    else if (counter == 2)
                    {
                        counter++;
                        Gyred += temp[currentX][currentY].rgbtRed * -2;
                        Gygreen += temp[currentX][currentY].rgbtGreen * -2;
                        Gyblue += temp[currentX][currentY].rgbtBlue * -2;
                    }
                    else if (counter == 4 || counter == 5 || counter == 6)
                    {
                        counter++;
                        continue;
                    }
                    else if (counter == 7 || counter == 9)
                    {
                        counter++;
                        Gyred += temp[currentX][currentY].rgbtRed * 1;
                        Gygreen += temp[currentX][currentY].rgbtGreen * 1;
                        Gyblue += temp[currentX][currentY].rgbtBlue * 1;
                    }
                    else if (counter == 8)
                    {
                        counter++;
                        Gyred += temp[currentX][currentY].rgbtRed * 2;
                        Gygreen += temp[currentX][currentY].rgbtGreen * 2;
                        Gyblue += temp[currentX][currentY].rgbtBlue * 2;
                    }
                }
            }
            //compute sqrt(Gx^2+Gy^2)
            int sumr , sumg , sumb;
            sumr = sumg = sumb = 0;
            sumr = (int) round(sqrt((Gxred * Gxred) + (Gyred * Gyred)));
            sumg = (int) round(sqrt((Gxgreen * Gxgreen) + (Gygreen * Gygreen)));
            sumb = (int) round(sqrt((Gxblue * Gxblue) + (Gyblue * Gyblue)));
                //limit the max value to 255
                if (sumr > 255)
                {
                    sumr = 255;
                }
                if (sumg > 255)
                {
                    sumg = 255;
                }
                if (sumb > 255)
                {
                    sumb = 255;
                }
        //sub that value back into image
        image[i][j].rgbtRed = sumr;
        image[i][j].rgbtGreen = sumg;
        image[i][j].rgbtBlue = sumb;
        }
    }
    return;
}
