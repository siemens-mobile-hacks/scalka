#include <swilib.h>
#include "config.h"

CONFIG CFG = {
    {CFG_CBOX, "Angle", 0, 3},
    0,
    {{"Degrees"}, {"Radians"}, {"Grads"}},
    {CFG_STR_WIN1251, "Fmt str", 1, 15},
    "%1.15lg",
    {CFG_CHECKBOX, "Enable Real-Time calculation", 0, 2},
    1,
};

char CFG_PATH[] = "?:\\zbin\\etc\\SCalka.bcfg";

void InitConfig() {
    CFG_PATH[0] = BCFG_GetDefaultDisk();
    if (BCFG_LoadConfig(CFG_PATH, &CFG, sizeof(CFG)) == -1) {
        BCFG_SaveConfig(CFG_PATH, &CFG, sizeof(CFG));
    }
}
