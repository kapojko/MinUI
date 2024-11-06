#ifndef MENU_H
#define MENU_H

#include "UIElement.h"

#define MAX_MENU_DISPLAY_ITEMS 4

// Menu flags
#define MENU_EXIT_AFTER_SCROLL 0b1
#define MENU_NARROW_CURSOR 0b10

#define MENU_EXIT_ACTION 9000

struct MenuItem {
    int action;
    char text[MAX_UI_TEXT_LENGTH];
};

struct Menu {
    const struct MenuItem(*items)[];
    int itemCount;

    int selectedItem;
    int displayOffset;
    int maxDisplayCount;
    int displayCount;

    short x;
    short y;
    short width;
    short height;

    short lineHeight;
    short lrMargin;

    int flags;

    struct UIElement cursorElement[MAX_MENU_DISPLAY_ITEMS];
    struct UIElement itemElement[MAX_MENU_DISPLAY_ITEMS];
    struct UIElement bottomFillElement;
    struct UIElement leftMarginElement;
    struct UIElement rightMarginElement;
};

enum MenuInput {
    MENU_INPUT_NONE,
    MENU_INPUT_UP,
    MENU_INPUT_DOWN,
    MENU_INPUT_ENTER,
    MENU_INPUT_EXIT
};

void InitMenu(struct Menu *menu, const struct MenuItem(*items)[],
    short x, short y, short width, short height, short lineHeight, short lrMargin,
    int flags);

void PrepareMenu(struct Menu *menu);
void PrepareMenuRepaintOnly(struct Menu *menu);

int ProcessMenuInput(struct Menu *menu, enum MenuInput input);
void UpdateMenu(struct Menu *menu);

#endif // MENU_H
