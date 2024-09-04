#include <stdio.h>

int main(int argc, char **argv)
{
    const char *image_path;
    const char *output_path;

    if (argc <= 2) {
        printf("Usage: %s <image path> <output path>\n", argv[0]);
        return 1;
    }

    return 0;
}
