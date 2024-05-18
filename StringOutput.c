#include <stdbool.h>
#include "MinUI.h"
#include "Font.h"
#include "StringOutput.h"

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
    for (; *str; str++) {
        int curChar = *str;

        // Get font image for the current character
        const tImage *image = GetCharImage(font, curChar);
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
            int dataSize = image->width * image->height / image->dataSize;
            int res = minuiPlatform->OutputPreparedBitmap(curX, y, image->width, image->height, image->data, dataSize);
            if (res != 0) {
                minuiPlatform->DebugPrint("Failed to output character image: %d\r\n", res);
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
