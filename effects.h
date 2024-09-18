#ifndef EFFECTS_H
#define EFFECTS_H

#include "image.h"

float lerp(float v0, float v1, float t);
Image *crossfade(Image *image1, Image *image2, float fade_factor);

#endif
