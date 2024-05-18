#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <stdbool.h>
#include "Font.h"

#define MAX_UI_TEXT_LENGTH 20

#define UI_ELEMENT_FLAG_CENTERED 0b1

enum UIElementType {
    UI_ELEMENT_TYPE_TEXT,
    UI_ELEMENT_TYPE_BITMAP,
    UI_ELEMENT_TYPE_FILL
};

struct UIElementText {
    char text[MAX_UI_TEXT_LENGTH];
};

struct UIElementBitmap {
    const tImage *image;
};

struct UIElementFill {
    int value;
};

struct UIElement {
    enum UIElementType type;
    bool painted;
    int flags;

    short x;
    short y;
    short width;
    short height;

    union {
        struct UIElementText text;
        struct UIElementBitmap bitmap;
        struct UIElementFill fill;
    };
};

void InitUIElement(struct UIElement *e, enum UIElementType type, int x, int y, int width, int height, int flags);
void PaintUIElement(struct UIElement *e);

void UpdateTextUIElement(struct UIElement *e, const char *text);
void UpdateBitmapUIElement(struct UIElement *e, const tImage *image);
void UpdateFillUIElement(struct UIElement *e, int value);

#endif // UIELEMENT_H
