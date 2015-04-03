#pragma once
#include "onut.h"

const UINT MENU_FILE_NEW = 101;
const UINT MENU_FILE_OPEN = 102;
const UINT MENU_FILE_SAVE = 103;
const UINT MENU_FILE_SAVE_AS = 104;
const UINT MENU_FILE_EXIT = 105;

const UINT MENU_EDIT_UNDO = 201;
const UINT MENU_EDIT_REDO = 202;
const UINT MENU_EDIT_CUT = 203;
const UINT MENU_EDIT_COPY = 204;
const UINT MENU_EDIT_PASTE = 205;
const UINT MENU_EDIT_DELETE = 206;

void buildMenu();
void onMenu(UINT index);
void checkShortCut(uintptr_t key);
