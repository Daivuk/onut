#include "ActionManager.h"
#include "DocumentView.h"
#include "menu.h"

extern DocumentView*        g_pDocument;
extern onut::ActionManager  g_actionManager;

onut::UIControl*    g_pMessageBox = nullptr;

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

    {
        auto subMenu = CreatePopupMenu();
        InsertMenu(subMenu, 0, MF_BYPOSITION | MF_STRING, MENU_EDIT_UNDO, TEXT("&Undo\tCtrl+Z"));
        InsertMenu(subMenu, 1, MF_BYPOSITION | MF_STRING, MENU_EDIT_REDO, TEXT("&Redo\tCtrl+Shift+Z"));
        InsertMenu(subMenu, 2, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr);
        InsertMenu(subMenu, 3, MF_BYPOSITION | MF_STRING, MENU_EDIT_CUT, TEXT("&Cut\tCtrl+X"));
        InsertMenu(subMenu, 4, MF_BYPOSITION | MF_STRING, MENU_EDIT_COPY, TEXT("&Copy\tCtrl+C"));
        InsertMenu(subMenu, 5, MF_BYPOSITION | MF_STRING, MENU_EDIT_PASTE, TEXT("&Paste\tCtrl+V"));
        InsertMenu(subMenu, 6, MF_BYPOSITION | MF_STRING, MENU_EDIT_DELETE, TEXT("&Delete\tDel"));
        InsertMenu(menu, 1, MF_BYPOSITION | MF_POPUP, (UINT)subMenu, TEXT("&Edit"));
    }

    SetMenu(window, menu);
    UpdateWindow(window);

    g_pMessageBox = new onut::UIControl("../../assets/ui/messageBox.json");
    g_pMessageBox->retain();
}

std::string fileOpen()
{
    auto window = OWindow->getHandle();
    char szFileName[MAX_PATH] = "";

    OPENFILENAMEA ofn = {0};
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = window;

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = window;
    ofn.lpstrFilter = "JSon Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = "json";

    GetOpenFileNameA(&ofn);

    return ofn.lpstrFile;
}

extern onut::ActionManager  g_actionManager;

void onMenu(UINT menuId)
{
    if (g_pMessageBox->getParent()) return;

    switch (menuId)
    {
        case MENU_FILE_NEW: // New
        {
//            showMessageBox("New Document", "Save changes?", 
            g_actionManager.clear();
            delete g_pDocument;
            g_pDocument = new DocumentView("");
            break;
        }
        case MENU_FILE_OPEN: // Open
        {
            auto filename = fileOpen();
            if (!filename.empty())
            {
                g_actionManager.clear();
                delete g_pDocument;
                g_pDocument = new DocumentView(filename);
            }
            break;
        }
        case MENU_FILE_SAVE: // Save
            g_pDocument->save();
            break;
        case MENU_FILE_SAVE_AS: // Save As
            break;
        case MENU_FILE_EXIT: // Exit
            PostQuitMessage(0);
            break;
        case MENU_EDIT_UNDO:
            g_actionManager.undo();
            break;
        case MENU_EDIT_REDO:
            g_actionManager.redo();
            break;
        case MENU_EDIT_CUT:
            g_pDocument->cut();
            break;
        case MENU_EDIT_COPY:
            g_pDocument->copy();
            break;
        case MENU_EDIT_PASTE:
            g_pDocument->paste();
            break;
        case MENU_EDIT_DELETE:
            g_pDocument->del();
            break;
    }
}

void checkShortCut(uintptr_t key)
{
    if (g_pMessageBox->getParent()) return;

    if (OInput->isStateDown(DIK_LCONTROL))
    {
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
        else if (key == static_cast<uintptr_t>('Z'))
        {
            if (OInput->isStateDown(DIK_LSHIFT))
            {
                onMenu(MENU_EDIT_REDO);
            }
            else
            {
                onMenu(MENU_EDIT_UNDO);
            }
        }
        else if (key == static_cast<uintptr_t>('X'))
        {
            onMenu(MENU_EDIT_CUT);
        }
        else if (key == static_cast<uintptr_t>('C'))
        {
            onMenu(MENU_EDIT_COPY);
        }
        else if (key == static_cast<uintptr_t>('V'))
        {
            onMenu(MENU_EDIT_PASTE);
        }
    }
    else if (key == VK_DELETE)
    {
        onMenu(MENU_EDIT_DELETE);
    }
}
