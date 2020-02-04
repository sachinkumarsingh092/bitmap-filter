#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_DEF_MAX_MIN 1

#ifdef IS_DEF_MAX_MIN

    #define max(a, b) (\
	    {__typeof__ (a) _a = (a);\
	    __typeof__ (b)_b = (b);\
	    (_a > _b) ? _a : _b;})

    #define min(a, b) (\
	    {__typeof__ (a) _a = (a);\
	    __typeof__ (b) _b = (b);\
	    (_a < _b) ? _a : _b;})
#endif

static void swap(RGBTRIPLE *, RGBTRIPLE *);

static void swap(RGBTRIPLE *pix1, RGBTRIPLE *pix2){
    RGBTRIPLE temp;
    temp = *pix1;
    *pix1 = *pix2;
    *pix2 = temp;
}

// Sobel convolution matrix/weights.
int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each pixel
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){

            // average of the RGB values for intensities of gray
            BYTE avg = (image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen)/3;

            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width/2; j++){
            swap(&image[i][j], &image[i][width-j-1]);
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{


    // Use a copy of images to make changes and apply change to image.

    /**
    RGBTRIPLE temp_img[height][width];

    use loop to make a copy
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            temp_img[i][j] = image[i][j];
        }

    or use memcpy from <string.h>
    memcpy(temp_img, image, sizeof(RGBTRIPLE)*height*width);
    }
    */

    RGBTRIPLE (*temp_img)[width] = image; 


    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){

            // use `int` and not `BYTE` if you want to save a few hours:(
            int avg_b = 0, avg_g = 0, avg_r = 0;
            int count = 0;
            for(int i = max(0, h-1); i <= min(height-1, h+1); i++){
                for(int j = max(0, w-1); j <= min(width-1, w+1); j++){
                    count++;
                    avg_b += temp_img[i][j].rgbtBlue;
                    avg_g += temp_img[i][j].rgbtGreen;
                    avg_r += temp_img[i][j].rgbtRed; 
                }
            }
            // put average into the original.
            image[h][w].rgbtBlue = (BYTE)(avg_b/count);
            image[h][w].rgbtGreen = (BYTE)(avg_g/count);
            image[h][w].rgbtRed = (BYTE)(avg_r/count);
        }
    }

    return;
}

// Detect edges
// see https://en.wikipedia.org/wiki/Sobel_operator
void edges(int height, int width, RGBTRIPLE image[height][width])
{
     // Allocate memory to keep track of original data
    int orig_height = height,  orig_width = width;
    RGBTRIPLE(*temp_img)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    memcpy(temp_img, image, sizeof(RGBTRIPLE) * height * width);
 
    height--;
    width--;
 
    while (height + 1)
    {
        while (width + 1)
        {
            // Gx and Gy variables for each color for the current pixel
            int x_Green_channel = 0, x_Blue_channel = 0, x_Red_channel = 0;
            int y_Green_channel = 0, y_Blue_channel = 0, y_Red_channel = 0;
 
            // Iterate over all pixels in a 3x3 square around the current pixel
            for (int i = width - 1; i <= width + 1; i++)
            {
                for (int j = height - 1; j <= height + 1; j++)
                {
                    // Determine indices for Gx/Gy matrices
                    int gw = 1, gh = 1;
                    if (i != width)
                    {
                        gw = i < width ? 0 : 2;
                    }
                    if (j != height)
                    {
                        gh = j < height ? 0 : 2;
                    }
 
                    // If border is black pixel
                    if (i < 0 || j < 0 || i >= orig_width || j >= orig_height)
                    {
                        x_Green_channel += Gx[gw][gh] * 0;
                        x_Blue_channel += Gx[gw][gh] * 0;
                        x_Red_channel += Gx[gw][gh] * 0;
 
                        y_Green_channel += Gy[gw][gh] * 0;
                        y_Blue_channel += Gy[gw][gh] * 0;
                        y_Red_channel += Gy[gw][gh] * 0;
                    }
                    else
                    {
                        x_Green_channel += Gx[gw][gh] * temp_img[j][i].rgbtRed;
                        x_Blue_channel += Gx[gw][gh] * temp_img[j][i].rgbtGreen;
                        x_Red_channel += Gx[gw][gh] * temp_img[j][i].rgbtBlue;
 
                        y_Green_channel += Gy[gw][gh] * temp_img[j][i].rgbtRed;
                        y_Blue_channel += Gy[gw][gh] * temp_img[j][i].rgbtGreen;
                        y_Red_channel += Gy[gw][gh] * temp_img[j][i].rgbtBlue;
                    }
                }
            }
 
            // Combine Gx and Gy to sqrt(Gx^2 + Gy^2) and cap at 255
            int Gxy_Red = min(255, round(sqrt(pow(x_Green_channel, 2) + pow(y_Green_channel, 2))));
            int Gxy_Green = min(255, round(sqrt(pow(x_Blue_channel, 2) + pow(y_Blue_channel, 2))));
            int Gxy_Blue = min(255, round(sqrt(pow(x_Red_channel, 2) + pow(y_Red_channel, 2))));
 
            image[height][width].rgbtRed = (BYTE)(Gxy_Red);
            image[height][width].rgbtGreen = (BYTE)(Gxy_Green);
            image[height][width].rgbtBlue = (BYTE)(Gxy_Blue);
 
            width--;
        }
        width = orig_width - 1;
        height--;
    }
    free(temp_img);
    return;
}