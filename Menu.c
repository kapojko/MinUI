#include "Menu.h"

static int getMenuItemCount(struct Menu *menu) {
    int count = 0;
    while ((*menu->items)[count].action != 0) {
        count++;
    }
    return count;
}

void InitMenu(struct Menu *menu, const struct MenuItem(*items)[],
        short x, short y, short width, short height, short lineHeight, short lrMargin) {
    // setup menu items
    menu->items = items;
    menu->itemCount = getMenuItemCount(menu);

    // setup current item
    menu->selectedItem = 0;
    menu->displayOffset = 0;

    // calculate the number of display items
    menu->maxDisplayCount = height / lineHeight;
    menu->displayCount = menu->itemCount < menu->maxDisplayCount ? menu->itemCount : menu->maxDisplayCount;

    // setup geometry
    menu->x = x;
    menu->y = y;
    menu->width = width;
    menu->height = height;
    menu->lineHeight = lineHeight;
    menu->lrMargin = lrMargin;

    // init display elements
    for (int i = 0; i < menu->displayCount; i++) {
        InitUIElement(&menu->cursorElement[i], UI_ELEMENT_TYPE_TEXT,
                menu->lrMargin, i * menu->lineHeight,
                menu->lineHeight, menu->lineHeight,
                0);
        InitUIElement(&menu->itemElement[i], UI_ELEMENT_TYPE_TEXT,
                menu->lrMargin + menu->lineHeight, i * menu->lineHeight,
                menu->width - 2 * menu->lrMargin - menu->lineHeight, menu->lineHeight,
                0);
    }

    // init bottom fill element
    if (menu->displayCount < menu->maxDisplayCount) {
        InitUIElement(&menu->bottomFillElement, UI_ELEMENT_TYPE_FILL,
                menu->lrMargin, menu->displayCount * menu->lineHeight,
                menu->width - 2 * menu->lrMargin, (menu->maxDisplayCount - menu->displayCount) * menu->lineHeight,
                0);
    }

    // init margin elements
    if (menu->lrMargin > 0) {
        InitUIElement(&menu->leftMarginElement, UI_ELEMENT_TYPE_FILL,
                0, 0,
                menu->lrMargin, menu->height,
                0);
        InitUIElement(&menu->rightMarginElement, UI_ELEMENT_TYPE_FILL,
                menu->width - menu->lrMargin, 0,
                menu->lrMargin, menu->height,
                0);
    }
}

void PrepareMenu(struct Menu *menu) {
    menu->selectedItem = 0;
    menu->displayOffset = 0;

    for (int i = 0; i < menu->displayCount; i++) {
        menu->cursorElement[i].painted = false;
        menu->itemElement[i].painted = false;
    }

    if (menu->displayCount < menu->maxDisplayCount) {
        menu->bottomFillElement.painted = false;
    }

    if (menu->lrMargin > 0) {
        menu->leftMarginElement.painted = false;
        menu->rightMarginElement.painted = false;
    }
}

int ProcessMenuInput(struct Menu *menu, enum MenuInput input) {
    int action = 0;

    // Update selected item
    switch (input) {
    case MENU_INPUT_NONE:
        break;
    case MENU_INPUT_UP:
        if (menu->selectedItem > 0) {
            menu->selectedItem--;

            if (menu->selectedItem < menu->displayOffset) {
                menu->displayOffset = menu->selectedItem;
            }
        }
        break;
    case MENU_INPUT_DOWN:
        if (menu->selectedItem < menu->itemCount - 1) {
            menu->selectedItem++;

            if (menu->selectedItem >= menu->displayOffset + menu->displayCount) {
                menu->displayOffset = menu->selectedItem - menu->displayCount + 1;
            }
        }
        break;
    case MENU_INPUT_ENTER:
        action = (*menu->items)[menu->selectedItem].action;
        break;
    case MENU_INPUT_EXIT:
        break;
    default:
        break;
    }

    return action;
}

void UpdateMenu(struct Menu *menu) {
    // Update line and cursor elements based on cursor position
    for (int i = 0; i < menu->displayCount; i++) {
        // Update cursor
        if (i == menu->selectedItem - menu->displayOffset) {
            UpdateTextUIElement(&menu->cursorElement[i], ">");
        } else {
            UpdateTextUIElement(&menu->cursorElement[i], "");
        }

        // Update line
        UpdateTextUIElement(&menu->itemElement[i], (*menu->items)[menu->displayOffset + i].text);
    }

    // Update bottom fill element
    if (menu->displayCount < menu->maxDisplayCount) {
        UpdateFillUIElement(&menu->bottomFillElement, 0);
    }
    
    // Update (paint) margin elements
    if (menu->lrMargin > 0) {
        UpdateFillUIElement(&menu->leftMarginElement, 0);
        UpdateFillUIElement(&menu->rightMarginElement, 0);
    }
}
