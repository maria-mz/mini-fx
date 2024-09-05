#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ffmpeg.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void create_video_from_img(char *image_path, char *video_name, int seconds)
{
    int w, h, ch;
    unsigned char *img = stbi_load(image_path, &w, &h, &ch, 3);

    if (img == NULL) {
        printf("failed to load image\n");
        exit(1);
    }

    FFMPEGWriteParams params = {
        w,
        h,
        "1",
        "yuv420p",
        "libx264",
        video_name
    };

    FFMPEGProcess *ffmpeg = open_ffmpeg_write_pipe(&params);

    if (ffmpeg == NULL) {
        printf("something went wrong...");
        exit(1);
    };

    for (int i = 0; i < seconds; ++i) {
        write(ffmpeg->pipe, img, w*h*ch*sizeof(unsigned char));
    }

    close(ffmpeg->pipe);
}

int main(int argc, char *argv[])
{
    char *image_path;
    char *video_name;

    if (argc != 3) {
        printf("Usage: %s <image path> <video name>\n", argv[0]);
        return 1;
    }

    image_path = argv[1];
    video_name = argv[2];

    create_video_from_img(image_path, video_name, 10);

    return 0;
}
