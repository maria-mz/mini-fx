#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "image.h"

Image *load_image(char *image_path) {
    Image *image = (Image*)malloc(sizeof(Image));

    if (image == NULL) {
        printf("failed to allocate image\n");
        return NULL;
    }

    image->data = stbi_load(
        image_path,
        &image->width,
        &image->height,
        &image->channels,
        CHANNELS
    );

    if (image->data == NULL) {
        printf("failed to load image data\n");
        unload_image(image);
        return NULL;
    }

    image->size = image->width *image->height * image->channels * sizeof(unsigned char);

    return image;
}

void unload_image(Image *image) {
    stbi_image_free(image->data);
    free(image);
}
