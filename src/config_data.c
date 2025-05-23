#include <cfg_items.h>

__CFG_BEGIN(0)

__root CFG_HDR cfghdr_0 = {CFG_CBOX, "Angle", 0, 3};
__root int CFG_DRG = 0;
__root CFG_CBOX_ITEM cfgcbox_0[] = {{"Degrees"}, {"Radians"}, {"Grads"}};

__root CFG_HDR cfghdr_1={CFG_STR_WIN1251, "Fmt str", 1, 15};
__root char CFG_FMT_STR[16] = "%1.15lg";

__root CFG_HDR cfghdr_2 = {CFG_CHECKBOX, "Enable Real-Time calculation", 0, 2};
__root int CFG_ENABLE_RT_CALC = 1;

__CFG_END(0)
