
/*
   Simple tools to manipulate images from/to pgm format;
   
    Author: Olivier BRUN
*/
#include "image.h"

/***********************************************************************************************
 * memory related functions
************************************************************************************************/

image *alloc_image(uint32_t width, uint32_t height) {
    image *new_img = malloc(sizeof(image));
    if(new_img == NULL ) { fprintf(stderr, "Memory allocation failed."); }

    new_img->height = height;
    new_img->width = width;
    
    new_img->data = malloc(height * width * sizeof(double));  // allocating tab of lines
    if(new_img->data == NULL ) { fprintf(stderr, "Memory allocation failed."); }
    
    return new_img;
}

void free_image(image *img) {
    free(img->data);
    free(img);
}



/***********************************************************************************************
 * pgm files related functions  (for grayscale images)
************************************************************************************************/

/*  Skip whites and comments in a PGM file. */
static void skip_whites_and_comments(FILE * f)
{
    int c;
    do
    {
        while(isspace(c=getc(f))); /* skip spaces */
        if(c=='#') /* skip comments */
            while( c!='\n' && c!='\r' && c!=EOF )
                c=getc(f);
    }
    while( c == '#' || isspace(c) );
    if( c != EOF && ungetc(c,f) == EOF )
        fprintf(stderr, "Error: unable to 'ungetc' while reading PGM file.");
}

/*  Read a ASCII number from a PGM file. */
static uint32_t get_num(FILE * f)
{
    uint32_t num;
    int c;

    while(isspace(c=getc(f)));
    if(!isdigit(c)) fprintf(stderr, "Error: corrupted PGM file.");
    num = (uint32_t) (c - '0');
    while( isdigit(c=getc(f)) ) num = 10 * num + c - '0';
    if( c != EOF && ungetc(c,f) == EOF )
        fprintf(stderr, "Error: unable to 'ungetc' while reading PGM file.");

    return num;
}

image *read_pgm_image(const char *filename) {
    uint32_t width, height, max_size;
    FILE * f;
    int c, bin; // temp var

    // Opening file...
    if ((f = fopen(filename, "rb")) == NULL) {
        fprintf(stderr, "Error: unable to open input image file %s",filename);
    }

    // Reading header
    if( getc(f) != 'P' ) fprintf(stderr, "Error: not a PGM file %s",filename);
    if( (c=getc(f)) == '2' ) bin = 0;
    else if( c == '5' ) bin = 1;
    else fprintf(stderr, "Error: not a PGM file %s",filename);
    skip_whites_and_comments(f);
    width = get_num(f);            /* X size */
    skip_whites_and_comments(f);
    height = get_num(f);            /* Y size */
    skip_whites_and_comments(f);
    max_size = get_num(f);          /* depth */

    if(max_size==0) {
        fprintf(stderr, "Warning: depth is zero, probably invalid PGM file\n");
    }
    //  white before data
    if(!isspace(c=getc(f))) {
    fprintf(stderr, "Error: corrupted PGM file %s",filename);
    }

    // Allocating memory for the new image
    image *img = alloc_image(width, height);

    // Reading image values
    if (!bin) {
        for (size_t i = 0; i < height; i++) 
            for (size_t j = 0; j < width; j++)
                (img->data)[i * width + j] = (double) get_num(f);
    }
    else { 
        for (size_t i = 0; i < height; i++) 
            for (size_t j = 0; j < width; j++) {
                // Splitting case where values are encoded with 1 and 2 bytes
                uint32_t val = (max_size < 256) ? getc(f) : getc(f)*256 + (double)getc(f);
                (img->data)[i * width + j] = (double) val;
            }
    }
    // Closing file
    if( f != stdin && fclose(f) == EOF )
        fprintf(stderr, "Error: unable to close file while reading PGM file %s",filename);

    return img;
}

void write_pgm_image(const char * filename, image * img) {
    FILE *f;
    double min = 0;
    double max = 0;
    uint32_t nlines = img->height;
    uint32_t ncol = img->width;

    /* Opening file */
    if (!strcmp(filename, "stdout"))
    {
        f = stdout;
    }
    else
    {
        f = fopen(filename, "w");
    }
        if( f == NULL )
            fprintf(stderr, "Error: unable to open output image file %s", filename);

    /* Computing max and min values */
    int val;
    for (size_t i = 0; i < nlines; i++)
        for (size_t j = 0; j < ncol; j++) {
            val = (int) (img->data)[i * ncol + j];
            max = (val > max) ? val : max;
            min = (val < min) ? val : min;
        }

    /* Writing a warning message if any value is negative or uppe than 255 */
    if (min < 0)
        fprintf(stderr, "Warning, image %s contains negative values (truncated to zero)", filename); 
    if (max > 255)
        fprintf(stderr, "Warning, image %s contains values upper than 255", filename);
    
    /* Writing pgm header */
    fprintf(f,"P5\n");
    fprintf(f,"%u %u\n", ncol, nlines);
    fprintf(f,"%d\n", 255);

    /* Writing pgm body */
    for(size_t i = 0; i < nlines; i++)
            for(size_t j = 0; j < ncol; j++)
                fputc((unsigned char) (img->data)[i * ncol + j], f);
    
    /* Closing file */
    if (f != stdout && fclose(f) == EOF)
        fprintf(stderr, "Error while closing file %s", filename);
}