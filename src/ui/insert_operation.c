#include <swilib.h>
#include <stdlib.h>
#include "../settings.h"
#include "../operation.h"
#include "keydesc.h"
#include "insert_operation.h"

typedef struct {
    GUI gui;
    UI_DATA *ui_data;
    WSHDR *ws;
    int state;
} MAIN_GUI;

static RECT CANVAS = {0, 0, 0, 0};

extern CALC_SETTINGS SETTINGS;

static inline int getXXXXwidth(int font) {
    return (GetSymbolWidth('X', font) * 4);
}

static void OnRedraw(MAIN_GUI* data) {
    int scr_w = ScreenW();
    int scr_h = ScreenH();
    int need_height;
    int need_width;
    int start_y;
    int start_x;
    int XXXXwidth = getXXXXwidth(FONT_SMALL);
    int Ysize = GetFontYSIZE(FONT_SMALL);

    need_height = Ysize * 4 + 5 * 2 * 3 + 5 * 2;
    need_width = XXXXwidth * 3 + 5 * 2 * 2 + 5 * 2;

    start_y = need_height < scr_h ? (scr_h - need_height) >> 1 : 0;
    start_x = need_width < scr_w ? (scr_w - need_width) >> 1 : 0;

    DrawRoundedFrame(start_x, start_y, start_x + need_width, start_y + need_height, 3, 3, 0,
                     GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(20));

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 3; x++) {
            unsigned int x_frame = start_x + XXXXwidth * x + 5 * 2 * x;
            unsigned int y_frame = start_y + Ysize * y + 5 * 2 * y;


            wsprintf(data->ws, "%t", KEY_DESC[data->state][y*3+x]);

            unsigned int str_width = Get_WS_width(data->ws, FONT_SMALL);
            unsigned int x_str = x_frame + ((XXXXwidth - str_width) >> 1) + 5;

            DrawRoundedFrame(x_frame, y_frame, x_frame + XXXXwidth + 5 * 2, y_frame + Ysize + 5 * 2, 3, 3, 0,
                             GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(20));
            DrawString(data->ws, x_str, y_frame + 5, x_str + str_width, y_frame + Ysize + 5,
                       FONT_SMALL, 0, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(23));
        }
    }
}

static void OnCreate(MAIN_GUI *data, malloc_func_t) {
    data->ws = AllocWS(128);
    data->gui.state = 1;
}

static void OnClose(MAIN_GUI *data, mfree_func_t) {
    data->gui.state = 0;
    FreeWS(data->ws);
}

static void OnFocus(MAIN_GUI *data, malloc_func_t, mfree_func_t) {
    DisableIDLETMR();
    data->gui.state = 2;
}

static void OnUnFocus(MAIN_GUI *data, malloc_func_t) {
    if (data->gui.state != 2) {
        return;
    }
    data->gui.state = 1;
}
static int OnKey(MAIN_GUI *data, GUI_MSG *msg) {
    const int submess = msg->gbsmsg->submess;
    if (msg->gbsmsg->msg == KEY_DOWN) {
        if (((submess >= '0') && (submess <= '9')) || (submess == '*')) {
            insert_operation(GetOperationIndexByKey(submess) + ((data->state) * 12));
            if (SETTINGS.auto_recalc) {
                data->ui_data->req_recalc = 1;
            }
            return 1;
        }
        else if (submess == '#') {
            if (++data->state > 3) {
                return 1;
            } else {
                DirectRedrawGUI();
                return -1;
            }
        }
        else if (submess == RIGHT_SOFT) {
            return 1;
        }
    }
    return 0;
}

static int method8(void) {
    return 0;
}

static int method9(void) {
    return 0;
}

const void * const gui_methods[11]={
    (void *)OnRedraw,
    (void *)OnCreate,
    (void *)OnClose,
    (void *)OnFocus,
    (void *)OnUnFocus,
    (void *)OnKey,
    0,
    (void *)kill_data,
    (void *)method8,
    (void *)method9,
    0
};

int InsertOperation_CreateUI(UI_DATA *ui_data) {
    MAIN_GUI *main_gui = malloc(sizeof(MAIN_GUI));
    zeromem(main_gui, sizeof(MAIN_GUI));
    main_gui->ui_data = ui_data;

    CANVAS.x2 = ScreenW() - 1;
    CANVAS.y2 = ScreenH() - 1;

    main_gui->gui.canvas = &CANVAS;
    main_gui->gui.flag30 = 2;
    main_gui->gui.methods = (void *)gui_methods;
    main_gui->gui.item_ll.data_mfree = (void (*)(void *))mfree_adr();
    main_gui->state = 1;
    return CreateGUI(main_gui);
}
