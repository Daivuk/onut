#include "DocumentView.h"
#include "menu.h"

extern DocumentView* g_pDocument;

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
        InsertMenu(subMenu, 0, MF_BYPOSITION | MF_STRING, MENU_FILE_NEW, TEXT("&New\tCtrl+N"));
        InsertMenu(subMenu, 1, MF_BYPOSITION | MF_STRING, MENU_FILE_OPEN, TEXT("&Open\tCtrl+O"));
        InsertMenu(subMenu, 2, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr);
        InsertMenu(subMenu, 3, MF_BYPOSITION | MF_STRING, MENU_FILE_SAVE, TEXT("&Save\tCtrl+S"));
        InsertMenu(subMenu, 4, MF_BYPOSITION | MF_STRING, MENU_FILE_SAVE_AS, TEXT("Save &As\tCtrl+Shift+S"));
        InsertMenu(subMenu, 5, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr);
        InsertMenu(subMenu, 6, MF_BYPOSITION | MF_STRING, MENU_FILE_EXIT, TEXT("E&xit\tAlt+F4"));
        InsertMenu(menu, 0, MF_BYPOSITION | MF_POPUP, (UINT)subMenu, TEXT("&File"));
    }
    
    SetMenu(window, menu);
    UpdateWindow(window);
}

std::string fileOpen()
{
    auto window = OWindow->getHandle();
    char szFileName[MAX_PATH] = "";

    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = window;

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = window;
    ofn.lpstrFilter = TEXT("JSon Files (*.json)\0*.json\0All Files (*.*)\0*.*\0");
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = TEXT("json");

    GetOpenFileName(&ofn);

    return ofn.lpstrFile;
}

//std::string fileOpen()
//{
//    auto window = OWindow->getHandle();
//    char szFileName[MAX_PATH] = "";
//
//    OPENFILENAME ofn = {0};
//    ofn.lStructSize = sizeof(OPENFILENAME);
//    ofn.hwndOwner = window;
//
//    ofn.lStructSize = sizeof(ofn);
//    ofn.hwndOwner = window;
//    ofn.lpstrFilter = TEXT("JSon Files (*.json)\0*.json\0All Files (*.*)\0*.*\0");
//    ofn.lpstrFile = szFileName;
//    ofn.nMaxFile = MAX_PATH;
//    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
//    ofn.lpstrDefExt = TEXT("json");
//
//    GetOpenFileName(&ofn);
//
//    return ofn.lpstrFile;
//}

void onMenu(UINT menuId)
{
    switch (menuId)
    {
        case MENU_FILE_NEW: // New
            break;
        case MENU_FILE_OPEN: // Open
        {
            auto filename = fileOpen();
            if (!filename.empty())
            {
                delete g_pDocument;
                g_pDocument = new DocumentView(filename);
            }
            break;
        }
        case MENU_FILE_SAVE: // Save
            break;
        case MENU_FILE_SAVE_AS: // Save As
            break;
        case MENU_FILE_EXIT: // Exit
            PostQuitMessage(0);
            break;
    }
}

void checkShortCut(uintptr_t key)
{
    if (OInput->isStateDown(DIK_LCONTROL))
    {
        if (key != 17)
        {
            int tmp;
            tmp = 6;
        }
        if (key == static_cast<uintptr_t>('N'))
        {
            onMenu(MENU_FILE_NEW);
        }
        else if (key == static_cast<uintptr_t>('O'))
        {
            onMenu(MENU_FILE_OPEN);
        }
        else if (key == static_cast<uintptr_t>('S'))
        {
            if (OInput->isStateDown(DIK_LSHIFT))
            {
                onMenu(MENU_FILE_SAVE_AS);
            }
            else
            {
                onMenu(MENU_FILE_SAVE);
            }
        }
    }
}
