#include <stddef.h>
#include "MinUI.h"
#include "Font.h"

const tImage *GetCharImage(const tFont *font, long int code) {
    for (int i = 0; i < font->length; i++) {
        if (font->chars[i].code == code) {
            return font->chars[i].image;
        }
    }

    return NULL;
}

const tFont* GetFontForHeight(int height) {
    for (int i = 0; minuiPlatform->fontData[i]; i++) {
        if (minuiPlatform->fontData[i]->height == height) {
            return minuiPlatform->fontData[i]->font;
        }
    }

    return NULL;
}
