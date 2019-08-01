#include "image.h"

int main() {
    image *img = alloc_image(10, 10);
    double *data = img->data;
    (img->data)[0] = 1;
    (img->data)[15] = 10;
    (img->data)[99] = 255;
    free_image(img);
}