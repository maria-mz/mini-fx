#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "ffmpeg.h"

FFMPEGProcess *open_ffmpeg_write_pipe(FFMPEGWriteParams *params) {
    int fd[2];
    
    if (pipe(fd) != 0) {
        printf("failed to create pipe\n");
        return NULL;
    }

    pid_t pid = fork();

    if (pid == -1) {
        printf("failed to create child ffmpeg process\n");
        return NULL;
    }

    if (pid == 0) {
        // Child process
        if (close(fd[WRITE_END]) != 0) {
            printf("failed to close write end in child ffmpeg process\n");
            exit(1);
        }

        if (dup2(fd[READ_END], STDIN_FILENO)) {
            printf("failed to redirect stdin to read end in child ffmpeg process\n");
            exit(1);
        }

        // TODO: may also want to redirect STDOUT because ffmpeg outputs a lot

        close(fd[READ_END]);

        char size[32];
        snprintf(size, sizeof(size), "%dx%d", params->width, params->height);

        int ret = execlp(
            "ffmpeg",
            "ffmpeg",
            "-y",
            "-f", "rawvideo",
            "-pix_fmt", "rgb24",     // TODO: maybe also parametrize this
            "-s", size,
            "-r", params->fps,
            "-i", "-",               // read from STDIN
            "-c:v", params->video_encoder,
            "-pix_fmt", params->pix_fmt,
            "-an",                  // for now, no audio, TODO
            params->video_name,
            NULL
        );

        // if success, rest of the lines shouldn't execute

        if (ret < 0) {
            printf("failed to execute ffmpeg process\n");
            exit(1);
        }
    }

    // Parent process
    if (close(fd[READ_END]) != 0) {
        printf("failed to close write end in parent ffmpeg process\n");
    }

    FFMPEGProcess *ffmpeg = (FFMPEGProcess*)malloc(sizeof(FFMPEGProcess));

    if (ffmpeg == NULL) {
        return NULL;
    }

    ffmpeg->pipe = fd[WRITE_END];
    ffmpeg->pid = pid;

    return ffmpeg;
}
