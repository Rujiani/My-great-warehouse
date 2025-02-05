#ifndef UI_HPP
#define UI_HPP

#include <string>
#include <ncurses.h>
#include "../logic/warehouse.hpp" // Use mgw::warehouse

// UI class handles all ncurses I/O and user interaction
class UI {
public:
    // Constructor with warehouse reference
    UI(mgw::warehouse& warehouseRef);

    // update() returns false when user selects "Exit"
    bool update();

private:
    mgw::warehouse& warehouse;
    int currentSelection;
    const int menuOptionsCount;
    // Menu options list
    const char* menuOptions[5];

    // UI action handlers
    void registerNewProduct();
    void sellProduct();
    void showAllProducts();
    void showMissingProducts();

    // Helper input functions
    std::string promptString(const char* prompt);
    size_t promptSizeT(const char* prompt);
};

#endif // UI_HPP
