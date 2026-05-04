#include "menu.h"


struct Menu {
    Menu(uint8_t id, uint8_t length): id(id), length(length) {}

    void selectNext() {
        current = (current + 1) % length;
    }

    void selectPrev() {
        current = length - (length - current) % length - 1;
    }

    uint8_t getCurrent() {
        return current;
    }

    uint8_t getId() {
        return id;
    }

private:
    uint8_t id = 0, length = 0, current = 0;
} menus[] = {
    // Menu(1, 3),     // main menu
    // Menu(2, 6),     // effects menu
    // Menu(3, 6),     // settings menu
    // Menu(4, 5),     // effect settings menu
};

static uint8_t active_menu = 0;


Menu* getCurrentMenu() {
    return (active_menu) ? &menus[active_menu - 1] : 0;
}


void setCurrentMenu(uint8_t menu_id) {
    active_menu = menu_id;
}
