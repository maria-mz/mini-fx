#include <stdlib.h>
#include "crossfade_composer.h"
#include "effects.h"

CrossfadeComposer *alloc_crossfade_composer(int fps, int duration)
{
    CrossfadeComposer *composer = (CrossfadeComposer*)malloc(sizeof(CrossfadeComposer));

    if (composer == NULL) {
        return NULL;
    }

    composer->curr_frame = 0;
    composer->total_frames = fps * duration;
    composer->fade_step = (float)1/composer->total_frames;

    return composer;
}

void free_crossfade_composer(CrossfadeComposer *composer)
{
    free(composer);
}

int send_frame(CrossfadeComposer *composer, Image *frame_from, Image *frame_to)
{
    if (done(composer)) {
        return -1;
    } else if (composer->out_frame) {
        return -2;
    }

    float fade_factor = composer->fade_step * composer->curr_frame;

    Image *out_frame = crossfade(frame_from, frame_to, fade_factor);

    composer->out_frame = out_frame;
    composer->curr_frame++;

    return 0;
}

Image *receive_frame(CrossfadeComposer *composer)
{
    Image *frame = composer->out_frame;
    composer->out_frame = NULL;
    return frame;
}

bool done(CrossfadeComposer *composer)
{
    return composer->curr_frame == composer->total_frames;
}
