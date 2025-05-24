#include <swilib.h>
#include <string.h>
#include "../settings.h"
#include "../operation.h"
#include "ui.h"
#include "icons.h"

#define ITEMS_N 5

static HEADER_DESC HEADER_D = {{0, 0, 0, 0},NULL, (int)LGP_OPTIONS, LGP_NULL};

static const int SOFTKEYS[] = {SET_LEFT_SOFTKEY, SET_RIGHT_SOFTKEY, SET_MIDDLE_SOFTKEY};

static const SOFTKEY_DESC SOFTKEYS_D[]= {
    {0x0018,0x0000,(int)LGP_SELECT},
    {0x0001,0x0000,(int)LGP_BACK},
    {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB SOFTKEYS_TAB = {
    SOFTKEYS_D,0
};

static void AC_Proc(GUI *gui) {
    remove_all_operation();
    GeneralFuncF1(1);
}

static void Ans2X_Proc(GUI *gui) {
    SETTINGS.x = D_ANSWER;
    GeneralFuncF1(1);
}

static void Ans2Y_Proc(GUI *gui) {
    SETTINGS.y = D_ANSWER;
    GeneralFuncF1(1);
}

static void Settings_Proc(GUI *gui) {
    extern char CFG_PATH[];
    WSHDR ws;
    uint16_t wsbody[128];
    CreateLocalWS(&ws, wsbody, 127);
    str_2ws(&ws, CFG_PATH, 127);
    ExecuteFile(&ws, NULL, NULL);
    GeneralFuncF1(1);
}

static void Exit_Proc(GUI *gui) {
    void *data = MenuGetUserPointer(gui);
    UI_DATA *ui_data = EDIT_GetUserPointer(data);

    GeneralFuncF1(1);
    GeneralFunc_flag1(ui_data->gui_id, 1);
}

static int ICON[] = {ICON_EMPTY};

static MENUITEM_DESC ITEMS[ITEMS_N] = {
    {ICON, (int)"AC", LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
    {ICON, (int)"ANS->X", LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
    {ICON, (int)"ANS->Y", LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
    {ICON, (int)LGP_SETTINGS, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
    {ICON, (int)LGP_EXIT, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2}
};

static const MENUPROCS_DESC PROCS[ITEMS_N] =  {
    AC_Proc,
    Ans2X_Proc,
    Ans2Y_Proc,
    Settings_Proc,
    Exit_Proc
};

static const MENU_DESC MENU_D = {
    8,
    NULL,
    NULL,
    NULL,
    SOFTKEYS,
    &SOFTKEYS_TAB,
    MENU_FLAGS_ENABLE_TEXT_SCROLLING | MENU_FLAGS_ENABLE_ICONS,
    NULL,
    ITEMS,
    PROCS,
    ITEMS_N
};

int MenuOptions_Create(GUI *gui) {
    RECT *rc = GetOptionsHeaderRect();
    memcpy(&(HEADER_D.rc), rc, sizeof(RECT));

    return CreateMenu(1, 0, &MENU_D, &HEADER_D, 0,ITEMS_N, gui, 0);
}
