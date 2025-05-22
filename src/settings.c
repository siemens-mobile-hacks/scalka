#include <swilib.h>
#include "settings.h"

#include <string.h>

extern int CFG_DRG;
extern const char CFG_FMT_STR[];
extern int CFG_ENABLE_RT_CALC;

CALC_SETTINGS calc_set;

void InitSettings() {
    calc_set.drg = CFG_DRG;
    strcpy(calc_set.fmt, CFG_FMT_STR);
    calc_set.auto_recalc = CFG_ENABLE_RT_CALC;
}
