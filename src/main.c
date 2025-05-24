#include <swilib.h>
#include <string.h>
#include "ui/ui.h"
#include "settings.h"
#include "conf_loader.h"

typedef struct {
    CSM_RAM csm;
    int gui_id;
} MAIN_CSM;

const int minus11 =- 11;
unsigned short maincsm_name_body[20];

static int CSM_OnMessage(CSM_RAM *data, GBS_MSG *msg) {
    extern char CFG_PATH[];

    MAIN_CSM *csm = (MAIN_CSM*)data;
    if (msg->msg == MSG_GUI_DESTROYED) {
        if ((int)msg->data0 == csm->gui_id) {
           csm->csm.state =- 3;
        }
    } else if (msg->msg == MSG_RECONFIGURE_REQ) {
        if (strcmp(CFG_PATH, msg->data0) == 0) {
          InitConfig();
          InitSettings();
          ShowMSG(1, (int)"SCalka config updated!");
        }
      }
    return 1 ;
}

static void CSM_OnCreate(CSM_RAM *data) {
    MAIN_CSM *csm = (MAIN_CSM*)data;
    csm->gui_id = CreateUI();
}

static void CSM_OnClose(CSM_RAM *) {
    SUBPROC(kill_elf);
}

const struct {
    CSM_DESC maincsm;
    WSHDR maincsm_name;
} MAINCSM = {
    {
        CSM_OnMessage,
        CSM_OnCreate,
#ifdef NEWSGOLD
        0,
        0,
        0,
        0,
#endif
        CSM_OnClose,
        sizeof(MAIN_CSM),
        1,
        &minus11
    },
    {
        maincsm_name_body,
        NAMECSM_MAGIC1,
        NAMECSM_MAGIC2,
        0x0,
        139
    }
};

void UpdateCSMname(void) {
    wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "%t", LGP_CALCULATOR);
}

int main() {
    MAIN_CSM csm;
    UpdateCSMname();
    InitConfig();
    InitSettings();
    LockSched();
    CreateCSM(&MAINCSM.maincsm, &csm, 0);
    UnlockSched();
    return 0;
}
