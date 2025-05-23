#include <swilib.h>
#include "settings.h"

#include <string.h>

extern int CFG_DRG;
extern const char CFG_FMT_STR[];
extern int CFG_ENABLE_RT_CALC;

CALC_SETTINGS SETTINGS;

void InitSettings() {
    SETTINGS.drg = CFG_DRG;
    strcpy(SETTINGS.fmt, CFG_FMT_STR);
    SETTINGS.auto_recalc = CFG_ENABLE_RT_CALC;
}
