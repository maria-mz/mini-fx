#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ffmpeg.h"
#include "image.h"
#include "effects.h"
#include "crossfade_composer.h"


// NOTE: This file is very experimental currently; using it to test structures
//        I'm building that will be part of the full implementation


void create_video_from_img(char *image_path, char *video_name, int seconds)
{
    Image *image = load_image(image_path);

    if (image == NULL) {
        printf("failed to load image\n");
        exit(1);
    }

    FFMPEGWriteParams params = {
        image->width,
        image->height,
        "1",
        "yuv420p",
        "libx264",
        video_name
    };

    FFMPEGProcess *ffmpeg = open_ffmpeg_write_pipe(&params);

    if (ffmpeg == NULL) {
        printf("error opening ffmpeg pipe\n");
        exit(1);
    };

    for (int i = 0; i < seconds; ++i) {
        write(ffmpeg->pipe, image->data, image->nb_bytes);
    }

    close(ffmpeg->pipe);
    unload_image(image);
}

void create_video_with_fade_transition() {
    Image *image = load_image("space.jpg");

    if (image == NULL) {
        printf("failed to load image\n");
        exit(1);
    }

    Image *image2 = load_image("river.jpg");

    if (image2 == NULL) {
        printf("failed to load image 2\n");
        exit(1);
    }

    FFMPEGWriteParams params = {
        image->width,
        image->height,
        "30",
        "yuv420p",
        "libx264",
        "fade.mp4"
    };

    FFMPEGProcess *ffmpeg = open_ffmpeg_write_pipe(&params);

    if (ffmpeg == NULL) {
        printf("error opening ffmpeg pipe\n");
        exit(1);
    };

    // creates a 2 second, 30 fps video with a fade duration of 2 seconds
    float fade_factor_step = (float)1/60;
    float fade_factor = 0;

    for (int i = 0; i <= 60; ++i) {
        Image *frame = crossfade(image, image2, fade_factor);

        if (frame == NULL) {
            printf("error creating output frame");
            exit(1);
        }

        write(ffmpeg->pipe, frame->data, frame->nb_bytes);
        unload_image(frame);

        fade_factor += fade_factor_step;
    }
}

void create_video_with_fade_transition_with_composer() 
{
    Image *image = load_image("space.jpg");

    if (image == NULL) {
        printf("failed to load image\n");
        exit(1);
    }

    Image *image2 = load_image("river.jpg");

    if (image2 == NULL) {
        printf("failed to load image 2\n");
        exit(1);
    }

    FFMPEGWriteParams params = {
        image->width,
        image->height,
        "30",
        "yuv420p",
        "libx264",
        "fade_composer.mp4"
    };

    FFMPEGProcess *ffmpeg = open_ffmpeg_write_pipe(&params);

    if (ffmpeg == NULL) {
        printf("something went wrong...\n");
        exit(1);
    };

    // creating a 2 second, 30 fps video, 60 frames
    CrossfadeComposer *composer = alloc_crossfade_composer(30, 2);

    for (int i = 0; i < 60; ++i) {
        int ret;

        if ((ret = send_frame(composer, image, image2)) != 0) {
            printf("error sending the next frame: %d, i = %d\n", ret, i);
        }

        Image *frame = receive_frame(composer);

        if (frame == NULL) {
            printf("error recieving the output frame, i = %d\n", i);
            exit(1);
        }

        write(ffmpeg->pipe, frame->data, frame->nb_bytes);
        unload_image(frame);
    }

    if (!done(composer)) {
        printf("composer should have finished!\n");
        exit(1);
    }
}

int main()
{
    // create_video_with_fade_transition();
    create_video_with_fade_transition_with_composer();

    return 0;
}
