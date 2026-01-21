#include "helpers.h"
#include <math.h>

int value_check(int x);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float average =
                (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / (float) 3;
            int averageR = round(average);
            image[i][j].rgbtRed = averageR;
            image[i][j].rgbtGreen = averageR;
            image[i][j].rgbtBlue = averageR;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sepiaRed = (image[i][j].rgbtRed * .393) + (image[i][j].rgbtGreen * .769) +
                             (image[i][j].rgbtBlue * .189);
            float sepiaGreen = (image[i][j].rgbtRed * .349) + (image[i][j].rgbtGreen * .686) +
                               (image[i][j].rgbtBlue * .168);
            float sepiaBlue = (image[i][j].rgbtRed * .272) + (image[i][j].rgbtGreen * .534) +
                              (image[i][j].rgbtBlue * .131);

            int sepiaRedR = round(sepiaRed);
            int sepiaGreenR = round(sepiaGreen);
            int sepiaBlueR = round(sepiaBlue);

            image[i][j].rgbtRed = value_check(sepiaRedR);
            image[i][j].rgbtGreen = value_check(sepiaGreenR);
            image[i][j].rgbtBlue = value_check(sepiaBlueR);
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int half = width / 2;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int k = 0; k < height; k++)
    {
        for (int l = 0; l < width; l++)
        {
            float averageRed = 0;
            float averageGreen = 0;
            float averageBlue = 0;

            // Dealing with the four corners
            if (k - 1 < 0 && l - 1 < 0) // Top-left pixel
            {
                averageRed = ((copy[k][l].rgbtRed) + (copy[k][l + 1].rgbtRed) +
                              (copy[l + 1][l].rgbtRed) + (copy[k + 1][l + 1].rgbtRed)) /
                             (float) 4;

                averageGreen = ((copy[k][l].rgbtGreen) + (copy[k][l + 1].rgbtGreen) +
                                (copy[k + 1][l].rgbtGreen) + (copy[k + 1][l + 1].rgbtGreen)) /
                               (float) 4;

                averageBlue = ((copy[k][l].rgbtBlue) + (copy[k][l + 1].rgbtBlue) +
                               (copy[k + 1][l].rgbtBlue) + (copy[k + 1][l + 1].rgbtBlue)) /
                              (float) 4;
            }
            else if (k - 1 < 0 && l + 1 >= width) // Top-right pixel
            {
                averageRed = ((copy[k][l - 1].rgbtRed) + (copy[k][l].rgbtRed) +
                              (copy[k + 1][l - 1].rgbtRed) + (copy[k + 1][l].rgbtRed)) /
                             (float) 4;

                averageGreen = ((copy[k][l - 1].rgbtGreen) + (copy[k][l].rgbtGreen) +
                                (copy[k + 1][l - 1].rgbtGreen) + (copy[k + 1][l].rgbtGreen)) /
                               (float) 4;

                averageBlue = ((copy[k][l - 1].rgbtBlue) + (copy[k][l].rgbtBlue) +
                               (copy[k + 1][l - 1].rgbtBlue) + (copy[k + 1][l].rgbtBlue)) /
                              (float) 4;
            }
            else if (k + 1 >= height && l - 1 < 0) // Bottom-left pixel
            {
                averageRed = ((copy[k - 1][l].rgbtRed) + (copy[k - 1][l + 1].rgbtRed) +
                              (copy[k][l].rgbtRed) + (copy[k][l + 1].rgbtRed)) /
                             (float) 4;

                averageGreen = ((copy[k - 1][l].rgbtGreen) + (copy[k - 1][l + 1].rgbtGreen) +
                                (copy[k][l].rgbtGreen) + (copy[k][l + 1].rgbtGreen)) /
                               (float) 4;

                averageBlue = ((copy[k - 1][l].rgbtBlue) + (copy[k - 1][l + 1].rgbtBlue) +
                               (copy[k][l].rgbtBlue) + (copy[k][l + 1].rgbtBlue)) /
                              (float) 4;
            }
            else if (k + 1 >= height && l + 1 >= width) // Bottom-right pixel
            {
                averageRed = ((copy[k - 1][l - 1].rgbtRed) + (copy[k - 1][l].rgbtRed) +
                              (copy[k][l - 1].rgbtRed) + (copy[k][l].rgbtRed)) /
                             (float) 4;

                averageGreen = ((copy[k - 1][l - 1].rgbtGreen) + (copy[k - 1][l].rgbtGreen) +
                                (copy[k][l - 1].rgbtGreen) + (copy[k][l].rgbtGreen)) /
                               (float) 4;

                averageBlue = ((copy[k - 1][l - 1].rgbtBlue) + (copy[k - 1][l].rgbtBlue) +
                               (copy[k][l - 1].rgbtBlue) + (copy[k][l].rgbtBlue)) /
                              (float) 4;
            }
            // Dealing with the four edges that aren't corners
            else if (l - 1 < 0) // Left edges
            {
                averageRed = ((copy[k - 1][l].rgbtRed) + (copy[k - 1][l + 1].rgbtRed) +
                              (copy[k][l].rgbtRed) + (copy[k][l + 1].rgbtRed) +
                              (copy[k + 1][l].rgbtRed) + (copy[k + 1][l + 1].rgbtRed)) /
                             (float) 6;

                averageGreen = ((copy[k - 1][l].rgbtGreen) + (copy[k - 1][l + 1].rgbtGreen) +
                                (copy[k][l].rgbtGreen) + (copy[k][l + 1].rgbtGreen) +
                                (copy[k + 1][l].rgbtGreen) + (copy[k + 1][l + 1].rgbtGreen)) /
                               (float) 6;

                averageBlue = ((copy[k - 1][l].rgbtBlue) + (copy[k - 1][l + 1].rgbtBlue) +
                               (copy[k][l].rgbtBlue) + (copy[k][l + 1].rgbtBlue) +
                               (copy[k + 1][l].rgbtBlue) + (copy[k + 1][l + 1].rgbtBlue)) /
                              (float) 6;
            }
            else if (k - 1 < 0) // Top edges
            {
                averageRed = ((copy[k][l - 1].rgbtRed) + (copy[k][l].rgbtRed) +
                              (copy[k][l + 1].rgbtRed) + (copy[k + 1][l - 1].rgbtRed) +
                              (copy[k + 1][l].rgbtRed) + (copy[k + 1][l + 1].rgbtRed)) /
                             (float) 6;

                averageGreen = ((copy[k][l - 1].rgbtGreen) + (copy[k][l].rgbtGreen) +
                                (copy[k][l + 1].rgbtGreen) + (copy[k + 1][l - 1].rgbtGreen) +
                                (copy[k + 1][l].rgbtGreen) + (copy[k + 1][l + 1].rgbtGreen)) /
                               (float) 6;

                averageBlue = ((copy[k][l - 1].rgbtBlue) + (copy[k][l].rgbtBlue) +
                               (copy[k][l + 1].rgbtBlue) + (copy[k + 1][l - 1].rgbtBlue) +
                               (copy[k + 1][l].rgbtBlue) + (copy[k + 1][l + 1].rgbtBlue)) /
                              (float) 6;
            }
            else if (l + 1 >= width) // Right edges
            {
                averageRed = ((copy[k - 1][l - 1].rgbtRed) + (copy[k - 1][l].rgbtRed) +
                              (copy[k][l - 1].rgbtRed) + (copy[k][l].rgbtRed) +
                              (copy[k + 1][l - 1].rgbtRed) + (copy[k + 1][l].rgbtRed)) /
                             (float) 6;

                averageGreen = ((copy[k - 1][l - 1].rgbtGreen) + (copy[k - 1][l].rgbtGreen) +
                                (copy[k][l - 1].rgbtGreen) + (copy[k][l].rgbtGreen) +
                                (copy[k + 1][l - 1].rgbtGreen) + (copy[k + 1][l].rgbtGreen)) /
                               (float) 6;

                averageBlue = ((copy[k - 1][l - 1].rgbtBlue) + (copy[k - 1][l].rgbtBlue) +
                               (copy[k][l - 1].rgbtBlue) + (copy[k][l].rgbtBlue) +
                               (copy[k + 1][l - 1].rgbtBlue) + (copy[k + 1][l].rgbtBlue)) /
                              (float) 6;
            }
            else if (k + 1 >= height) // Bottom edges
            {
                averageRed = ((copy[k - 1][l - 1].rgbtRed) + (copy[k - 1][l].rgbtRed) +
                              (copy[k - 1][l + 1].rgbtRed) + (copy[k][l - 1].rgbtRed) +
                              (copy[k][l].rgbtRed) + (copy[k][l + 1].rgbtRed)) /
                             (float) 6;

                averageGreen = ((copy[k - 1][l - 1].rgbtGreen) + (copy[k - 1][l].rgbtGreen) +
                                (copy[k - 1][l + 1].rgbtGreen) + (copy[k][l - 1].rgbtGreen) +
                                (copy[k][l].rgbtGreen) + (copy[k][l + 1].rgbtGreen)) /
                               (float) 6;

                averageBlue = ((copy[k - 1][l - 1].rgbtBlue) + (copy[k - 1][l].rgbtBlue) +
                               (copy[k - 1][l + 1].rgbtBlue) + (copy[k][l - 1].rgbtBlue) +
                               (copy[k][l].rgbtBlue) + (copy[k][l + 1].rgbtBlue)) /
                              (float) 6;
            }

            else // Dealing when the pixel is not on edge or on a corner
            {
                averageRed = ((copy[k - 1][l - 1].rgbtRed) + (copy[k - 1][l].rgbtRed) +
                              (copy[k - 1][l + 1].rgbtRed) + (copy[k][l - 1].rgbtRed) +
                              (copy[k][l].rgbtRed) + (copy[k][l + 1].rgbtRed) +
                              (copy[k + 1][l - 1].rgbtRed) + (copy[k + 1][l].rgbtRed) +
                              (copy[k + 1][l + 1].rgbtRed)) /
                             (float) 9;

                averageGreen = ((copy[k - 1][l - 1].rgbtGreen) + (copy[k - 1][l].rgbtGreen) +
                                (copy[k - 1][l + 1].rgbtGreen) + (copy[k][l - 1].rgbtGreen) +
                                (copy[k][l].rgbtGreen) + (copy[k][l + 1].rgbtGreen) +
                                (copy[k + 1][l - 1].rgbtGreen) + (copy[k + 1][l].rgbtGreen) +
                                (copy[k + 1][l + 1].rgbtGreen)) /
                               (float) 9;

                averageBlue = ((copy[k - 1][l - 1].rgbtBlue) + (copy[k - 1][l].rgbtBlue) +
                               (copy[k - 1][l + 1].rgbtBlue) + (copy[k][l - 1].rgbtBlue) +
                               (copy[k][l].rgbtBlue) + (copy[k][l + 1].rgbtBlue) +
                               (copy[k + 1][l - 1].rgbtBlue) + (copy[k + 1][l].rgbtBlue) +
                               (copy[k + 1][l + 1].rgbtBlue)) /
                              (float) 9;
            }

            int roundedRed = round(averageRed);
            int roundedGreen = round(averageGreen);
            int roundedBlue = round(averageBlue);

            image[k][l].rgbtRed = roundedRed;
            image[k][l].rgbtGreen = roundedGreen;
            image[k][l].rgbtBlue = roundedBlue;
        }
    }
}

int value_check(int x)
{
    const int min = 0;
    const int max = 255;

    if (x < min)
    {
        x = min;
        return x;
    }
    else if (x > max)
    {
        x = max;
        return x;
    }
    else
    {
        return x;
    }
}
