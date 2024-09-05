#include <stdio.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1

typedef struct FFMPEGProcess {
    int pipe;
    int pid;
} FFMPEGProcess;

typedef struct FFMPEGWriteParams {
    int width;
    int height;
    char *fps;
    char *pix_fmt;
    char *video_encoder;
    char *video_name;
} FFMPEGWriteParams;

FFMPEGProcess *open_ffmpeg_write_pipe(FFMPEGWriteParams *params);

// TODO: read pipe for reading in video frames
