#include <stdbool.h>
#include "MinUI.h"
#include "Font.h"
#include "Utf8.h"
#include "StringOutput.h"

static int calcImageDataSize(const tImage *image) {
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

static int processString(int x, int y, int width, int height, const char *str, bool calcWidthOnly) {
    // FIXME: use utf-8, see https://github.com/haipome/utf8/blob/master/utf8.c

    // select font based on height
    const tFont *font = GetFontForHeight(height);
    if (!font) {
        minuiPlatform->DebugPrint("Unsupported height: %d, failed to select font\r\n", height);
        return -1;
    }

    int curX = x;
    int curWidth = 0;
    long curCodepoint;
    for (const char *next = str; next; next = ReadUtf8Character(next, &curCodepoint)) {
        // Get font image for the current character
        const tImage *image = GetCharImage(font, curCodepoint);
        if (!image) {
            continue;
        }

        // Check remaining columns
        if (curX + image->width > x + width) {
            break;
        }

        if (!calcWidthOnly) {
            // Output image
            // NOTE: font image is already prepared for direct output
            int dataSize = calcImageDataSize(image);
            bool ok = minuiPlatform->OutputPreparedBitmap(curX, y, image->width, image->height, image->data, dataSize);
            if (!ok) {
                minuiPlatform->DebugPrint("Failed to output character image\r\n");
                return -1;
            }
        }

        curX += image->width;
        curWidth += image->width;
    }

    return curWidth;
}

int GetStringWidth(int x, int y, int width, int height, const char *str) {
    return processString(x, y, width, height, str, true);
}

int OutputString(int x, int y, int width, int height, const char *str) {
    return processString(x, y, width, height, str, false);
}
