#!/bin/bash

set -xe

CFLAGS="-Wall -Wextra"

clang $CFLAGS -o mini-fx main.c
