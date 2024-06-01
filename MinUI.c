#include "MinUI.h"

struct MinUIPlatform *minuiPlatform;

void InitMinUI(struct MinUIPlatform *platform) {
    minuiPlatform = platform;
}

int CalcImageDataSize(const tImage *image) {
    int widthBytes, heightBytes;

    switch (minuiPlatform->imageDataLayout) {
    case IMAGE_DATA_LAYOUT_WIDTH:
        widthBytes = (image->width % image->dataSize) ? (image->width / image->dataSize) + 1 : image->width / image->dataSize;
        heightBytes = image->height;
        return widthBytes * heightBytes;
    case IMAGE_DATA_LAYOUT_HEIGHT: 
        widthBytes = image->width;
        heightBytes = (image->height % image->dataSize) ? (image->height / image->dataSize) + 1 : image->height / image->dataSize;
        return widthBytes * heightBytes;
    default:
        return 0;
    }
}
