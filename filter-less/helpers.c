#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float averageFloat = (image[h][w].rgbtBlue + image[h][w].rgbtGreen + image[h][w].rgbtRed) / 3.0;
            int average = round(averageFloat);

            image[h][w].rgbtBlue = average;
            image[h][w].rgbtGreen = average;
            image[h][w].rgbtRed = average;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int originalBlue = image[h][w].rgbtBlue;
            int originalGreen = image[h][w].rgbtGreen;
            int originalRed = image[h][w].rgbtRed;

            float sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue;
            float sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue;
            float sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue;

            image[h][w].rgbtBlue = sepiaBlue < 255 ? round(sepiaBlue) : 255;
            image[h][w].rgbtGreen = sepiaGreen < 255 ? round(sepiaGreen) : 255;
            image[h][w].rgbtRed = sepiaRed < 255 ? round(sepiaRed) : 255;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reflected[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            reflected[h][width - (w + 1)] = image[h][w];
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = reflected[h][w];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurred[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float boxRed = 0;
            float boxGreen = 0;
            float boxBlue = 0;
            float counter = 0.0;

            // h and w represent the current pixel as reference
            for (int row = -1; row < 2; row++)
            {
                for (int col = -1; col < 2; col++)
                {
                    int boxHeight = h + row;
                    int boxWidth = w + col;

                    // Check if outside height or width. Skip pixel if true
                    if (boxHeight < 0 || boxHeight > height - 1 || boxWidth < 0 || boxWidth > width - 1)
                    {
                        continue;
                    }

                    boxRed += image[boxHeight][boxWidth].rgbtRed;
                    boxGreen += image[boxHeight][boxWidth].rgbtGreen;
                    boxBlue += image[boxHeight][boxWidth].rgbtBlue;
                    counter += 1.0;
                }
            }

            float finalRed = round(boxRed / counter);
            float finalGreen = round(boxGreen / counter);
            float finalBlue = round(boxBlue / counter);

            blurred[h][w].rgbtRed = finalRed < 255 ? finalRed : 255;
            blurred[h][w].rgbtGreen = finalGreen < 255 ? finalGreen : 255;
            blurred[h][w].rgbtBlue = finalBlue < 255 ? finalBlue : 255;
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = blurred[h][w];
        }
    }
    return;
}
