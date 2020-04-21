#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float grey;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            grey = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            image[i][j].rgbtBlue = (int) round(grey / 3);
            image[i][j].rgbtGreen = (int) round(grey / 3);
            image[i][j].rgbtRed = (int) round(grey / 3);
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sepiaRed = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            float sepiaGreen = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            float sepiaBlue = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);
            //
            if (sepiaBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = (int) sepiaBlue;
            }
            if (sepiaGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = (int) sepiaGreen;
            }
            if (sepiaRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = (int) sepiaRed;
            }
        }


    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int temp_Red, temp_Green, temp_Blue;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp_Red = image[i][j].rgbtRed;
            temp_Green = image[i][j].rgbtGreen;
            temp_Blue = image[i][j].rgbtBlue;

            image[i][j].rgbtRed =  image[i][width - 1 - j].rgbtRed;
            image[i][j].rgbtGreen =  image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtBlue =  image[i][width - 1 - j].rgbtBlue;

            image[i][width - 1 - j].rgbtRed = temp_Red;
            image[i][width - 1 - j].rgbtGreen = temp_Green;
            image[i][width - 1 - j].rgbtBlue = temp_Blue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sum_Blue = 0;
            float sum_Green = 0;
            float sum_Red = 0;
            float num = 0;

            for (int ii = i - 1 ; ii < i + 2 && ii < height; ii++)
            {
                if (ii < 0)
                {
                    continue;
                }
                for (int jj = j - 1; jj < j + 2 && jj< width; jj++)
                {
                    if (jj < 0)
                    {
                        continue;
                    }
                    sum_Blue += image[ii][jj].rgbtBlue;
                    sum_Green += image[ii][jj].rgbtGreen;
                    sum_Red += image[ii][jj].rgbtRed;
                    num ++;
                }
            }
            float avr_Blue = sum_Blue / num;
            float avr_Green = round(sum_Green / num);
            float avr_Red = round(sum_Red / num);

            temp[i][j].rgbtBlue = (int) round(avr_Blue);
            temp[i][j].rgbtGreen = avr_Green;
            temp[i][j].rgbtRed = avr_Red;

        }
    }
    for (int iii = 0; iii < width; iii++)
    {
        for (int jjj = 0; jjj < height; jjj++)
        {
            image[iii][jjj].rgbtBlue = temp[iii][jjj].rgbtBlue;
            image[iii][jjj].rgbtGreen = temp[iii][jjj].rgbtGreen;
            image[iii][jjj].rgbtRed = temp[iii][jjj].rgbtRed;
        }
    }


    return;
}
