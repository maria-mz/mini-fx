#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "effects.h"

// From https://en.wikipedia.org/wiki/Linear_interpolation
float lerp(float v0, float v1, float t)
{
    return (1.0f - t) * v0 + t * v1;
}

Image *crossfade(Image *image_from, Image *image_to, float fade_factor)
{
    if (
        image_from->width != image_to->width ||
        image_from->height != image_to->height ||
        image_from->channels != image_to->channels
    ) {
        printf("images must have the same dimensions and channels\n");
        return NULL;
    }

    Image *faded_image = (Image*)malloc(sizeof(Image));

    if (faded_image == NULL) {
        printf("failed to allocate image\n");
        return NULL;
    }

    copy_image_structure(image_from, faded_image);

    for (int y = 0; y < faded_image->height; ++y) {
        for (int x = 0; x < faded_image->width; ++x) {
            for (int c = 0; c < faded_image->channels; ++c) {
                int loc = (y * faded_image->width + x) * faded_image->channels + c;

                float pixel_from = image_from->data[loc];
                float pixel_to = image_to->data[loc];
                float faded_pixel = (unsigned char)lerp(pixel_from, pixel_to, fade_factor);

                faded_image->data[loc] = faded_pixel;
            }
        }
    }

    return faded_image;
}
