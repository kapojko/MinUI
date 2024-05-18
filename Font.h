#ifndef FONT_H
#define FONT_H

// lcd-image-converter font type

#include <stdint.h>

typedef struct {
    const uint8_t *data;
    uint16_t width;
    uint16_t height;
    uint8_t dataSize;
} tImage;
typedef struct {
    long int code;
    const tImage *image;
} tChar;
typedef struct {
    int length;
    const tChar *chars;
} tFont;

struct FontData {
    const tFont *font;
    uint16_t height;
};

const tImage *GetCharImage(const tFont *font, long int code);
const tFont* GetFontForHeight(int height);

#endif // FONT_H
