#include <string.h>
#include "MinUI.h"
#include "StringOutput.h"
#include "UIElement.h"

static void paintTextUIElement(struct UIElement *e) {
    int ret;

    short outputX = e->x;
    short outputWidth = e->width;
    int textWidth;

    // For centered text calculate width
    if (e->flags & UI_ELEMENT_FLAG_CENTERED) {
        textWidth = GetStringWidth(e->x, e->y, e->width, e->height, e->text.text);

        if (textWidth > 0 && textWidth < e->width) {
            outputX = e->x + (e->width - textWidth) / 2;
            outputWidth = textWidth;
        }
    }

    // Fill area on the left
    if (outputX > e->x) {
        ret = minuiPlatform->FillArea(e->x, e->y, outputX - e->x, e->height, 0x00);
        if (ret != 0) {
            minuiPlatform->DebugPrint("FillArea failed: %d\r\n", ret);
            return;
        }
    }

    // Output text
    // TODO: we can implement partial string refresh if only part of it has changed
    textWidth = OutputString(outputX, e->y, outputWidth, e->height, e->text.text);
    if (textWidth < 0) {
        return; // Error already printed
    }

    // Fill area on the right
    if (outputX + textWidth < e->x + e->width) {
        ret = minuiPlatform->FillArea(outputX + textWidth, e->y, e->x + e->width - outputX - textWidth, e->height, 0x00);
        if (ret != 0) {
            minuiPlatform->DebugPrint("FillArea failed: %d\r\n", ret);
            return;
        }
    }
}

static void paintBitmapUIElement(struct UIElement *e) {
    int ret;
    
    if (e->bitmap.image == NULL) {
        // Just fill the area
        ret = minuiPlatform->FillArea(e->x, e->y, e->width, e->height, 0xFF);
        if (ret != 0) {
            minuiPlatform->DebugPrint("FillArea failed: %d\r\n", ret);
        }
        return;
    }

    // Output bitmap
    int dataSize = e->bitmap.image->width * e->bitmap.image->height / e->bitmap.image->dataSize;
    ret = minuiPlatform->OutputPreparedBitmap(e->x, e->y, e->width, e->height, e->bitmap.image->data, dataSize);
    if (ret != 0) {
        minuiPlatform->DebugPrint("OutputPreparedBitmap failed: %d\r\n", ret);
    }
}

static void paintFillUIElement(struct UIElement *e) {
    int ret = minuiPlatform->FillArea(e->x, e->y, e->width, e->height, e->fill.value);
    if (ret != 0) {
        minuiPlatform->DebugPrint("FillArea failed: %d\r\n", ret);
    }
}

void InitUIElement(struct UIElement *e, enum UIElementType type, int x, int y, int width, int height, int flags) {
    e->type = type;
    e->painted = false;
    e->flags = flags;

    e->x = x;
    e->y = y;
    e->width = width;
    e->height = height;

    switch (type) {
    case UI_ELEMENT_TYPE_TEXT:
        e->text.text[0] = 0;
        break;
    case UI_ELEMENT_TYPE_BITMAP:
        e->bitmap.image = NULL;
        break;
    case UI_ELEMENT_TYPE_FILL:
        e->fill.value = 0xFF;
        break;
    }
}

void PaintUIElement(struct UIElement *e) {
    switch (e->type) {
    case UI_ELEMENT_TYPE_TEXT:
        paintTextUIElement(e);
        break;
    case UI_ELEMENT_TYPE_BITMAP:
        paintBitmapUIElement(e);
        break;
    case UI_ELEMENT_TYPE_FILL:
        paintFillUIElement(e);
        break;
    }

    e->painted = true;
}

void UpdateTextUIElement(struct UIElement *e, const char *text) {
    if (e->type != UI_ELEMENT_TYPE_TEXT) {
        return;
    }

    bool changed = (strcmp(e->text.text, text) != 0);
    if (!e->painted || changed) {
        // Update text
        strncpy(e->text.text, text, MAX_UI_TEXT_LENGTH);
        e->text.text[MAX_UI_TEXT_LENGTH - 1] = 0;

        // Paint UI element
        PaintUIElement(e);
    }
}

void UpdateBitmapUIElement(struct UIElement *e, const tImage *image) {
    if (e->type != UI_ELEMENT_TYPE_BITMAP) {
        return;
    }

    // NOTE: just compare pointers
    bool changed = (e->bitmap.image != image);
    if (!e->painted || changed) {
        // Update bitmap
        e->bitmap.image = image;

        // Paint UI element
        PaintUIElement(e);
    }
}

void UpdateFillUIElement(struct UIElement *e, int value) {
    if (e->type != UI_ELEMENT_TYPE_FILL) {
        return;
    }

    bool changed = (e->fill.value != value);
    if (!e->painted || changed) {
        // Update fill
        e->fill.value = value;

        // Paint UI element
        PaintUIElement(e);
    }
}
