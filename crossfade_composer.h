#ifndef CROSSFADE_COMPOSER_H
#define CROSSFADE_COMPOSER_H

#include <stdbool.h>
#include "image.h"

typedef struct CrossfadeComposer {
    Image *out_frame;

    int total_frames;
    int curr_frame;

    float fade_step;
} CrossfadeComposer;

CrossfadeComposer *alloc_crossfade_composer(int fps, int duration);
void free_crossfade_composer(CrossfadeComposer *composer);
int send_frame(CrossfadeComposer *composer, Image *frame_from, Image *frame_to);
Image *receive_frame(CrossfadeComposer *composer);
bool done(CrossfadeComposer *composer);

#endif
