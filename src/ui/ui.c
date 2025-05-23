#include "ui.h"

#include <locale.h>
#include <swilib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../operation.h"
#include "../settings.h"
#include "lgp.h"
#include "icons.h"
#include "keydesc.h"
#include "menu_options.h"
#include "insert_operation.h"

static int ICON[] = {ICON_CALCULATOR};

static HEADER_DESC HEADER_D = {{0, 0, 0, 0}, ICON, (int)LGP_CALCULATOR, LGP_NULL};

static SOFTKEY_DESC SOFTKEYS_D[] = {
    {0x0018, 0x0000, LGP_OPTIONS},
    {0x0001, 0x0000, LGP_EXIT},
    {0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static SOFTKEYSTAB SOFTKEYS = {
    SOFTKEYS_D, 0
};

static int OnKey(GUI *gui, GUI_MSG *msg) {
    UI_DATA *data = EDIT_GetUserPointer(gui);

    int focus = EDIT_GetFocus(gui);
    int submess = msg->gbsmsg->submess;
    if (msg->keys == 0x18) {
        MenuOptions_Create(gui);
        return -1;
    } else if (msg->keys == 0x01) {
        return 1;
    } else if (msg->keys == 0x3D) {
        REQ_RECALC:
            data->req_recalc = 1;
            return -1;
    } else if (msg->gbsmsg->msg == KEY_DOWN) {
        if (submess == GREEN_BUTTON) {
            goto REQ_RECALC;
        }
    }
    if (focus == 4) {
        if (msg->keys == 0xFFE) {
            remove_operation();
            if (SETTINGS.auto_recalc) {
                data->req_recalc = 1;
            }
            return -1;
        }
        if (msg->gbsmsg->msg == KEY_DOWN) {
            if (((submess >= '0') && (submess <= '9')) || (submess == '*')) {
                insert_operation(GetOperationIndexByKey(submess));
                if (SETTINGS.auto_recalc) {
                    data->req_recalc = 1;
                }
                return -1;
            }
            else if (submess == '#') {
                InsertOperation_CreateUI(data);
                return -1;
            }
        }
        if (msg->gbsmsg->msg == KEY_DOWN || msg->gbsmsg->msg == LONG_PRESS) {
            if (submess == LEFT_BUTTON) {
                if (OP_POS) OP_POS--;
                return -1;
            }
            if (submess == RIGHT_BUTTON) {
                if (OP_POS < OP_LEN) OP_POS++;
                return -1;
            }
        }
    }
    return -1;
}

void GHook(GUI *gui, int cmd) {
    UI_DATA *data= EDIT_GetUserPointer(gui);

    static SOFTKEY_DESC SK_DEL = {0x0FFE, 0x0FFE, 3272};
    if (cmd == TI_CMD_REDRAW) {
        WSHDR ws;
        uint16_t wsbody[256];
        CreateLocalWS(&ws, wsbody, 255);

        int focus = EDIT_GetFocus(gui);

        SetSoftKey(gui, &SOFTKEYS_D[0], SET_LEFT_SOFTKEY);
        SOFTKEYS_D[2].lgp_id = (SETTINGS.auto_recalc) ? LGP_NULL : LGP_DOIT_PIC;
        SetSoftKey(gui, &SOFTKEYS_D[2], SET_MIDDLE_SOFTKEY);
        if (focus == 2 || OP_POS == 0) {
            SetSoftKey(gui, &SOFTKEYS_D[1], SET_RIGHT_SOFTKEY);
        } else {
            SetSoftKey(gui, &SK_DEL, SET_RIGHT_SOFTKEY);
        }

        if (focus == 4) {
            int k = 0;
            if (OP_LEN) {
                uint8_t c;
                char* s = OPERATION;
                int i = 0;
                while (i < OP_LEN) {
                    const char* op;
                    c = *s++;
                    op = KEY_DESC[0][c];
                    while ((c = *op++)) {
                        wsAppendChar(&ws, c);
                        if (i < OP_POS) k++;
                    }
                    i++;
                }
            }
            EDIT_SetTextToEditControl(gui, 4, &ws);
            EDIT_SetCursorPos(gui, k + 1);
        }
        if (data->req_recalc) {
            char str[256];
            calc_answer();
            sprintf(str, SETTINGS.fmt, D_ANSWER);
            str_2ws(&ws, str, 255);
            EDIT_SetTextToEditControl(gui, 2, &ws);
            data->req_recalc = 0;
        }
    } else if (cmd == TI_CMD_CREATE) {
        EDIT_SetFocus(gui, 4);
    } else if (cmd == TI_CMD_DESTROY) {
        mfree(data);
    }
}

static INPUTDIA_DESC INPUTDIA_D = {
    8,
    OnKey,
    GHook,
    NULL,
    0,
    &SOFTKEYS,
    {0, 0, 0, 0},
    FONT_MEDIUM,
    100,
    101,
    0,
    0,
    INPUTDIA_FLAGS_SWAP_SOFTKEYS
};

int CreateUI(void) {
    RECT* header_rect = GetHeaderRECT();
    RECT* main_area_rect = GetMainAreaRECT();
    memcpy(&(HEADER_D.rc), header_rect, sizeof(RECT));
    memcpy(&(INPUTDIA_D.rc), main_area_rect, sizeof(RECT));

    UI_DATA *data = malloc(sizeof(UI_DATA));
    zeromem(data, sizeof(UI_DATA));

    void *malloc = malloc_adr();

    WSHDR ws;
    uint16_t wsbody[32];
    CreateLocalWS(&ws, wsbody, 31);

    EDITCONTROL ec;
    PrepareEditControl(&ec);
    void* eq = AllocEQueue(malloc, mfree_adr());

    wsprintf(&ws, "%t:", LGP_RESULT);
    ConstructEditControl(&ec, ECT_HEADER, ECF_APPEND_EOL, &ws, 31);
    AddEditControlToEditQend(eq, &ec, malloc);

    ConstructEditControl(&ec, ECT_READ_ONLY, ECF_APPEND_EOL, 0, 128);
    AddEditControlToEditQend(eq, &ec, malloc);

    wsprintf(&ws, "%s", "----------");
    ConstructEditControl(&ec, ECT_HEADER, ECF_APPEND_EOL, &ws, 31);
    AddEditControlToEditQend(eq, &ec, malloc);

    ConstructEditControl(&ec, ECT_NORMAL_TEXT, ECF_APPEND_EOL, 0, 128);
    AddEditControlToEditQend(eq, &ec, malloc);

    data->gui_id = CreateInputTextDialog(&INPUTDIA_D, &HEADER_D, eq, 1, data);
    return data->gui_id;
}
