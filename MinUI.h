#ifndef MINUI_H
#define MINUI_H

#include <stdint.h>
#include "Font.h"

#define MAX_FONTS 4

typedef int (*OutputPreparedBitmapFunc)(int x, int y, int width, int height, const uint8_t *bitmap, int dataSize);
typedef int (*FillAreaFunc)(int x, int y, int width, int height, uint8_t value);
typedef void (*DebugPrintFunc)(const char *fmt, ...);

struct MinUIPlatform {
    OutputPreparedBitmapFunc OutputPreparedBitmap;
    FillAreaFunc FillArea;
    DebugPrintFunc DebugPrint;

    const struct FontData *fontData[MAX_FONTS + 1];
};

extern struct MinUIPlatform *minuiPlatform;

void InitMinUI(struct MinUIPlatform *platform);

#endif // MINUI_H
