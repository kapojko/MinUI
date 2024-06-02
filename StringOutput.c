#include <stdbool.h>
#include "MinUI.h"
#include "Font.h"
#include "Utf8.h"
#include "StringOutput.h"

static int processString(int x, int y, int width, int height, const char *str, bool calcWidthOnly) {
    // FIXME: use utf-8, see https://github.com/haipome/utf8/blob/master/utf8.c

    // select font based on height
    const tFont *font = GetFontForHeight(height);
    if (!font) {
        minuiPlatform->debugPrint("Unsupported height: %d, failed to select font\r\n", height);
        return -1;
    }

    int curX = x;
    int curWidth = 0;

    const char *next = str;
    for (;;) {
        long curCodepoint;
        unsigned long curCodepointUtf8;
        next = ReadUtf8Character(next, &curCodepoint, &curCodepointUtf8);

        if (!next) {
            break;
        }

        // Get font image for the current character
        const tImage *image = GetCharImage(font, (long)curCodepointUtf8);
        if (!image) {
            minuiPlatform->debugPrint("Unsupported codepoint: %llx\r\n", curCodepointUtf8);
            continue;
        }

        // Check remaining columns
        if (curX + image->width > x + width) {
            break;
        }

        if (!calcWidthOnly) {
            // Output image
            // NOTE: font image is already prepared for direct output
            int dataSize = CalcImageDataSize(image);
            bool ok = minuiPlatform->outputPreparedBitmap(curX, y, image->width, image->height, image->data, dataSize);
            if (!ok) {
                minuiPlatform->debugPrint("Failed to output character image\r\n");
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
