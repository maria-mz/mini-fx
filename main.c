#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ffmpeg.h"
#include "image.h"
#include "transitions.h"

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
        printf("something went wrong...\n");
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
        printf("something went wrong...\n");
        exit(1);
    };

    // creates a 2 second, 30 fps video with a fade duration of 2 seconds
    float fade_factor_step = (float)1/60;
    float fade_factor = 0;

    for (int i = 0; i <= 60; ++i) {
        Image *frame = crossfade(image, image2, fade_factor);

        if (frame == NULL) {
            printf("something went wrong...");
            exit(1);
        }

        write(ffmpeg->pipe, frame->data, frame->nb_bytes);
        unload_image(frame);

        fade_factor += fade_factor_step;
    }
}

int main(int argc, char *argv[])
{
    create_video_with_fade_transition();
    // char *image_path;
    // char *video_name;

    // if (argc != 3) {
    //     printf("Usage: %s <image path> <video name>\n", argv[0]);
    //     return 1;
    // }

    // image_path = argv[1];
    // video_name = argv[2];

    // create_video_from_img(image_path, video_name, 10);

    return 0;
}
