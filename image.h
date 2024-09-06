#ifndef IMAGE_H
#define IMAGE_H

#define CHANNELS 3

typedef struct Image {
    int width;
    int height;
    int channels;
    int nb_bytes;
    unsigned char *data;
} Image;

Image *load_image(char *image_path);
void unload_image(Image *image);

#endif
