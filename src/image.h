/*
   Simple tools to manipulate images from/to pgm format;
   
    Author: Olivier BRUN
*/
#ifndef IMAGE
#define IMAGE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/*
    Structure of an image
*/
typedef struct 
{
    uint32_t width;
    uint32_t height;
    double *data;
} image;

/*   Allocate memory for a new image   */
image *alloc_image(uint32_t width, uint32_t height);

/*  Free an image previously allocated */
void free_image(image *img);

/* Load a pgm file */
image *read_pgm_image(const char *filename);

/* Save an image using pgm format
 * Use filename = "stdout" to use stdout.
 */
void write_pgm_image(const char * filename, image * img);

#endif