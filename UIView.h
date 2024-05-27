#ifndef UIVIEW_H
#define UIVIEW_H

#include "UIElement.h"

#define MAX_UI_ELEMENTS 32

struct UIView {
    struct UIElement *elements[MAX_UI_ELEMENTS]; // terminated with NULL pointer
    int elementCount;
};

void InitUIView(struct UIView *uiView, struct UIElement *elements[]);
void AddUIElement(struct UIView *uiView, struct UIElement *element);
void RemoveUIElement(struct UIView *uiView, struct UIElement *element);

void PreparePaintAll(struct UIView *uiView);

const char *UIView_UnitTest(void);

#endif // UIVIEW_H
