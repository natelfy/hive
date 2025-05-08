#include "menu.h"

int main() {
    using namespace Hive;

    Menu& menu = Menu::getMenu();
    menu.terminalInterface();
    return 0;

}
