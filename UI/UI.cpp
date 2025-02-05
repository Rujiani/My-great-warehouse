#include "UI.hpp"
#include <sstream>
#include <cstdlib>

// Constructor: initialize menu and selection index
UI::UI(mgw::warehouse& warehouseRef)
    : warehouse(warehouseRef), currentSelection(0), menuOptionsCount(5)
{
    menuOptions[0] = "1) Register a new product";
    menuOptions[1] = "2) Sell a product";
    menuOptions[2] = "3) Show all products";
    menuOptions[3] = "4) Show missing products";
    menuOptions[4] = "5) Exit";
}

// Main update loop: displays menu, handles arrow keys and Enter
bool UI::update() {
    int ch;
    clear();
    mvprintw(0, 0, "Warehouse Management TUI");

    // Draw menu options with current selection highlighted
    for (int i = 0; i < menuOptionsCount; ++i) {
        if (i == currentSelection)
            attron(A_REVERSE);
        mvprintw(2 + i, 2, "%s", menuOptions[i]);
        if (i == currentSelection)
            attroff(A_REVERSE);
    }
    refresh();

    ch = getch();
    switch(ch) {
        case KEY_UP:
            currentSelection = (currentSelection - 1 + menuOptionsCount) % menuOptionsCount;
            break;
        case KEY_DOWN:
            currentSelection = (currentSelection + 1) % menuOptionsCount;
            break;
        case 10: // Enter key
            switch(currentSelection) {
                case 0:
                    registerNewProduct();
                    break;
                case 1:
                    sellProduct();
                    break;
                case 2:
                    showAllProducts();
                    break;
                case 3:
                    showMissingProducts();
                    break;
                case 4:
                    return false; // Exit chosen
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return true;
}

// Helper: prompt for string input
std::string UI::promptString(const char* prompt) {
    echo(); // Enable echo
    char input[256];
    clear();
    mvprintw(0, 0, "%s", prompt);
    mvprintw(1, 0, "Input: ");
    getnstr(input, 255);
    noecho();
    return std::string(input);
}

// Helper: prompt for numeric input and convert to size_t
size_t UI::promptSizeT(const char* prompt) {
    std::string input = promptString(prompt);
    try {
        return static_cast<size_t>(std::stoul(input));
    } catch (...) {
        return 0;
    }
}

// Action: register a new product via user input
void UI::registerNewProduct() {
    std::string cipher = promptString("Enter product cipher:");
    mgw::product_components pr;
    pr.name     = promptString("Enter product name:");
    pr.firm     = promptString("Enter manufacturer:");
    pr.country  = promptString("Enter country of manufacture:");
    pr.type     = promptString("Enter product type (wholesale/retail):");
    pr.quantity = promptSizeT("Enter quantity:");
    pr.cost     = promptSizeT("Enter cost per unit:");
    pr.num      = promptSizeT("Enter additional identifier:");

    try {
        warehouse.register_product(cipher, pr);
        clear();
        mvprintw(0, 0, "Product registered successfully.");
    } catch (std::exception &e) {
        clear();
        mvprintw(0, 0, "Error: %s", e.what());
    }
    mvprintw(LINES - 2, 0, "Press any key to return to menu.");
    getch();
}

// Action: sell a product
void UI::sellProduct() {
    std::string cipher = promptString("Enter product cipher to sell:");
    size_t num = promptSizeT("Enter number of units to sell:");

    try {
        size_t salePrice = warehouse.sell_product(cipher, num);
        clear();
        mvprintw(0, 0, "Sale completed. Total sale price: %zu", salePrice);
    } catch (std::exception &e) {
        clear();
        mvprintw(0, 0, "Error: %s", e.what());
    }
    mvprintw(LINES - 2, 0, "Press any key to return to menu.");
    getch();
}

// Action: display all products (report)
void UI::showAllProducts() {
    std::string report = warehouse.get_report();
    clear();
    int row = 0;
    mvprintw(row++, 0, "All Products Report:");
    std::istringstream iss(report);
    std::string line;
    while (std::getline(iss, line) && row < LINES - 2) {
        mvprintw(row++, 0, "%s", line.c_str());
    }
    mvprintw(LINES - 2, 0, "Press any key to return to menu.");
    getch();
}

// Action: display missing products
void UI::showMissingProducts() {
    std::string missing = warehouse.missing_products();
    clear();
    int row = 0;
    mvprintw(row++, 0, "Missing Products:");
    std::istringstream iss(missing);
    std::string line;
    while (std::getline(iss, line) && row < LINES - 2) {
        mvprintw(row++, 0, "%s", line.c_str());
    }
    mvprintw(LINES - 2, 0, "Press any key to return to menu.");
    getch();
}
