#ifndef MINUI_H
#define MINUI_H

#include <stdint.h>
#include <stdbool.h>
#include "Font.h"

#define MAX_FONTS 10

enum ImageDataLayout {
    IMAGE_DATA_LAYOUT_WIDTH,
    IMAGE_DATA_LAYOUT_HEIGHT
};

typedef bool (*OutputPreparedBitmapFunc)(int x, int y, int width, int height, const uint8_t *data, int dataSize);
typedef bool (*FillAreaFunc)(int x, int y, int width, int height, int value);
typedef void (*DebugPrintFunc)(const char *fmt, ...);

struct MinUIPlatform {
    OutputPreparedBitmapFunc OutputPreparedBitmap;
    FillAreaFunc FillArea;
    DebugPrintFunc DebugPrint;

    const struct FontData *fontData[MAX_FONTS + 1];
    enum ImageDataLayout imageDataLayout;
    int bgValue;
};

extern struct MinUIPlatform *minuiPlatform;

void InitMinUI(struct MinUIPlatform *platform);

#endif // MINUI_H
