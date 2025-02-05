#include <ncurses.h>
#include "UI/UI.hpp"
#include "logic/warehouse.hpp" // Provided warehouse header in mgw namespace

int main() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Create warehouse instance (model)
    mgw::warehouse wh;

    // Create UI instance (view+controller)
    UI ui(wh);

    // Main update loop; update() returns false when "Exit" is chosen.
    bool running = true;
    while (running) {
        running = ui.update();
    }

    // End ncurses mode
    endwin();
    return 0;
}
