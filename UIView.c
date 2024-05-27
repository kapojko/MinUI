#include <stddef.h>
#include <string.h>
#include "MinUnit.h"
#include "UIView.h"

void InitUIView(struct UIView *uiView, struct UIElement *elements[]) {
    int cnt = 0;

    while (elements[cnt] != NULL) {
        if (cnt >= MAX_UI_ELEMENTS) {
            break;
        }
        
        uiView->elements[cnt] = elements[cnt];
        cnt++;
    }

    uiView->elementCount = cnt;
}

void AddUIElement(struct UIView *uiView, struct UIElement *element) {
    if (uiView->elementCount >= MAX_UI_ELEMENTS) {
        return;
    }

    uiView->elements[uiView->elementCount++] = element;
}

void RemoveUIElement(struct UIView *uiView, struct UIElement *element) {
    for (int i = 0; i < uiView->elementCount; i++) {
        if (uiView->elements[i] == element) {
            // Move subsequent elements back
            if (i < uiView->elementCount - 1) {
                memmove(&uiView->elements[i], &uiView->elements[i + 1], sizeof(struct UIElement *) * (uiView->elementCount - i - 1));
            }
            
            uiView->elementCount--;
            return;
        }
    }
}

void PreparePaintAll(struct UIView *uiView) {
    for (int i = 0; i < uiView->elementCount; i++) {
        uiView->elements[i]->painted = false;
    }
}

const char *UIView_UnitTest(void) {
    struct UIElement element1, element2, element3, element4;
    struct UIElement *elements[] = {&element1, &element2, &element3, NULL};

    // Test init
    struct UIView uiView;
    InitUIView(&uiView, elements);
    mu_assert("Element count should be 3", uiView.elementCount == 3);

    // Test add
    AddUIElement(&uiView, &element4);
    mu_assert("Element count should be 4", uiView.elementCount == 4);

    // Test remove
    RemoveUIElement(&uiView, &element1);
    mu_assert("Element count should be 3", uiView.elementCount == 3);

    // Test remove non-existent
    RemoveUIElement(&uiView, &element1);
    mu_assert("Element count should be still 3", uiView.elementCount == 3);

    // Test add above MAX (use the same element to add which is not absolutely correct but works for this test)
    for (int i = 0; i < MAX_UI_ELEMENTS + 1; i++) {
        AddUIElement(&uiView, &element1);
    }
    mu_assert("Element count should be MAX", uiView.elementCount == MAX_UI_ELEMENTS);

    return NULL;
}
