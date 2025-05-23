#include <swilib.h>
#include <stdlib.h>
#include <string.h>
#include "../settings.h"
#include "../operation.h"
#include "keydesc.h"
#include "insert_operation.h"

typedef struct {
    int state;
    WSHDR *ws;
    UI_DATA *ui_data;
} INSERT_OPERATION_DATA;

extern CALC_SETTINGS SETTINGS;

static void OnRedraw(GUI *) {
    GUI *gui = GetTopGUI();
    INSERT_OPERATION_DATA *data = GetPopupUserPointer(gui);

    int x = gui->canvas->x;
    int y = gui->canvas->y;
    int x2 = gui->canvas->x2;
    int y2 = gui->canvas->y2;

    int w = (x2 - x - 20) / 3;
    int h = (y2 - y - 20) / 4;

    x += 10;
    y += 10;
    const int font = FONT_SMALL;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            int x_item = x + w * i;
            int y_item = y + h * j;
            int x2_item = x_item + w;
            int y2_item = y_item + h;
            DrawRectangle(x_item, y_item, x2_item, y2_item, 0,
                GetPaletteAdrByColorIndex(PC_FOREGROUND), GetPaletteAdrByColorIndex(0x17));
            wsprintf(data->ws, "%t", KEY_DESC[data->state + 1][j * 3 + i]);

            int y_text = y_item + (h - GetFontYSIZE(font)) / 2;
            int y2_text = y_text + GetFontYSIZE(font);
            DrawString(data->ws, x_item, y_text, x2_item, y2_text, font, TEXT_ALIGNMIDDLE,
                GetPaletteAdrByColorIndex(PC_FOREGROUND), GetPaletteAdrByColorIndex(0x17));
        }
    }
}

static int OnKey(GUI *gui, GUI_MSG *msg) {
    INSERT_OPERATION_DATA *data = GetPopupUserPointer(gui);
    const int submess = msg->gbsmsg->submess;
    if (msg->gbsmsg->msg == KEY_DOWN) {
        if (((submess >= '0') && (submess <= '9')) || (submess == '*')) {
            insert_operation(GetOperationIndexByKey(submess) + ((data->state + 1) * 12));
            if (SETTINGS.auto_recalc) {
                data->ui_data->req_recalc = 1;
            }
            return 1;
        }
        else if (submess == '#') {
            data->state++;
            if (data->state < 3) {
                DirectRedrawGUI();
            } else {
                return 1;
            }
        }
        else if (submess == RIGHT_SOFT) {
            return 1;
        }
    }
    return -1;
}

static void GHook(GUI *gui, int cmd) {
    INSERT_OPERATION_DATA *data = GetPopupUserPointer(gui);

    if (cmd == TI_CMD_CREATE) {
        static GUI_METHODS gui_methods;
        void **m = GetDataOfItemByID(gui, 30);
        memcpy(&gui_methods, m[1], sizeof(GUI_METHODS));
        gui_methods.onRedraw = OnRedraw;
        m[1] = &gui_methods;
        data->ws = AllocWS(16);
    } else if (cmd == TI_CMD_DESTROY) {
        FreeWS(data->ws);
    }
}

const POPUP_DESC POPUP_D = {
    0,
    OnKey,
    GHook,
    NULL,
    NULL,
    NULL,
    1,
    LGP_NULL,
    NULL,
    0,
    FONT_SMALL,
    101,
    0,
};

int InsertOperation_CreateUI(UI_DATA *ui_data) {
    INSERT_OPERATION_DATA *data = malloc(sizeof(INSERT_OPERATION_DATA));
    zeromem(data, sizeof(INSERT_OPERATION_DATA));
    data->ui_data = ui_data;
    return CreatePopupGUI(1, data, &POPUP_D, LGP_NULL);
}
