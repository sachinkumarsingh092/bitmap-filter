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
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE (*temp_img)[width] = image; 


    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){
            
            int x_blue_channel = 0, x_green_channel = 0, x_red_channel = 0;
            int y_blue_channel = 0, y_green_channel = 0, y_red_channel = 0;
            int gh = 0;
            for(int i = h-1; i <= h+1; i++){
                int gw = 0;
                for(int j = w-1; j <= w+1; j++){
                    if((i < 0 || j < 0) || (i > height-1 || j > width-1)){
                        x_blue_channel += 0;
                        x_green_channel += 0;
                        x_red_channel += 0;

                        y_blue_channel += 0;
                        y_green_channel += 0;
                        y_red_channel += 0;

                        // or simpy continue
                    }

                    else{
                    // printf("%d %d => %d %d      ", i, j, gh, gw);
                        x_blue_channel += (temp_img[i][j].rgbtBlue)*Gx[gh][gw];
                        x_green_channel += (temp_img[i][j].rgbtGreen)*Gx[gh][gw];
                        x_red_channel += (temp_img[i][j].rgbtRed)*Gx[gh][gw];

                        y_blue_channel += (temp_img[i][j].rgbtBlue)*Gy[gh][gw];
                        y_green_channel += (temp_img[i][j].rgbtGreen)*Gy[gh][gw];
                        y_red_channel += (temp_img[i][j].rgbtRed)*Gy[gh][gw];
                    }
                    gw++;
                }
                // printf("\n");
                gh++;
            }
            // printf("\n");

            int G_blue = min(255, (int)sqrt( (x_blue_channel*x_blue_channel) + (y_blue_channel*y_blue_channel)));
            int G_green = min(255, (int)sqrt( (x_green_channel*x_green_channel) + (y_green_channel*y_green_channel)));
            int G_red = min(255, (int)sqrt( (x_red_channel*x_red_channel) + (y_red_channel*y_red_channel)));

            // put into the original.
            image[h][w].rgbtBlue = (BYTE)(G_blue);
            image[h][w].rgbtGreen = (BYTE)(G_green);
            image[h][w].rgbtRed = (BYTE)(G_red);
        }
    }

    return;
}
