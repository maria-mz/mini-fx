#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third_party/stb_image_write.h"

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

    image->nb_bytes = image->width
                    * image->height
                    * image->channels
                    * sizeof(unsigned char);

    return image;
}

void copy_image_structure(Image *image_from, Image* image_to)
{
    image_to->width = image_from->width;
    image_to->height = image_from->height;
    image_to->channels = image_from->channels;
    image_to->nb_bytes = image_from->width
                        * image_from->height
                        * image_from->channels
                        * sizeof(unsigned char);
    image_to->data = (unsigned char*)malloc(image_from->nb_bytes);
}

void save_image(Image *image, char *output_path)
{
    stbi_write_png(
        output_path,
        image->width,
        image->height,
        image->channels,
        image->data,
        image->width * image->channels
    );
}

void unload_image(Image *image) {
    stbi_image_free(image->data);
    free(image);
}
