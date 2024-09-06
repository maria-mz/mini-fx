#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "transitions.h"

// From https://en.wikipedia.org/wiki/Linear_interpolation
float lerp(float v0, float v1, float t)
{
    return (1.0f - t) * v0 + t * v1;
}

Image *crossfade(Image *image1, Image *image2, float fade_factor)
{
    if (
        image1->width != image2->width ||
        image1->height != image2->height ||
        image1->channels != image2->channels
    ) {
        printf("images must have the same dimensions and channels\n");
        return NULL;
    }

    Image *out = (Image*)malloc(sizeof(Image));

    if (out == NULL) {
        printf("failed to allocate image\n");
        return NULL;
    }

    out->width = image1->width;
    out->height = image1->height;
    out->channels = image1->channels;
    out->nb_bytes = out->width * out->height * out->channels * sizeof(unsigned char);
    out->data = (unsigned char*)malloc(out->nb_bytes);

    for (int y = 0; y < out->height; ++y) {
        for (int x = 0; x < out->width; ++x) {
            for (int c = 0; c < out->channels; ++c) {
                int loc = (y * out->width + x) * out->channels + c;
                float pixel1 = image1->data[loc];
                float pixel2 = image2->data[loc];
                out->data[loc] = (unsigned char)lerp(pixel1, pixel2, fade_factor);
            }
        }
    }

    return out;
}
