#!/bin/bash

set -xe

CFLAGS="-Wall -Wextra"

clang $CFLAGS -o mini-fx main.c ffmpeg.c image.c effects.c crossfade_composer.c
