#include "image.h"

int main() {
    image *img = read_pgm_image("lung1.pgm");
    write_pgm_image("copy_lung1.pgm", img);
    free_image(img);
}