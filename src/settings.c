#include <string.h>
#include "config.h"
#include "settings.h"

CALC_SETTINGS SETTINGS;

void InitSettings() {
    SETTINGS.drg = CFG.drg;
    strcpy(SETTINGS.fmt, CFG.fmt_str);
    SETTINGS.auto_recalc = CFG.enable_rt_calc;
}
