#include "menu.h"

const UINT MENU_FILE_NEW = 101;
const UINT MENU_FILE_OPEN = 102;
const UINT MENU_FILE_SAVE = 103;
const UINT MENU_FILE_SAVE_AS = 104;
const UINT MENU_FILE_EXIT = 105;

void buildMenu()
{
    auto window = OWindow->getHandle();
    auto menu = CreateMenu();

    {
        auto subMenu = CreatePopupMenu();
        InsertMenu(subMenu, 0, MF_BYPOSITION | MF_STRING, MENU_FILE_NEW, TEXT("&New"));
        InsertMenu(subMenu, 1, MF_BYPOSITION | MF_STRING, MENU_FILE_OPEN, TEXT("&Open"));
        InsertMenu(subMenu, 2, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr);
        InsertMenu(subMenu, 3, MF_BYPOSITION | MF_STRING, MENU_FILE_SAVE, TEXT("&Save"));
        InsertMenu(subMenu, 4, MF_BYPOSITION | MF_STRING, MENU_FILE_SAVE_AS, TEXT("Save &As"));
        InsertMenu(subMenu, 5, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr);
        InsertMenu(subMenu, 6, MF_BYPOSITION | MF_STRING, MENU_FILE_EXIT, TEXT("E&xit"));
        InsertMenu(menu, 0, MF_BYPOSITION | MF_POPUP, (UINT)subMenu, TEXT("&File"));
    }

    SetMenu(window, menu);
    UpdateWindow(window);
}

void onMenu(UINT menuId)
{
    switch (menuId)
    {
        case MENU_FILE_NEW: // New
            break;
        case MENU_FILE_OPEN: // Open
            break;
        case MENU_FILE_SAVE: // Save
            break;
        case MENU_FILE_SAVE_AS: // Save As
            break;
        case MENU_FILE_EXIT: // Exit
            PostQuitMessage(0);
            break;
    }
}
