#pragma once

#include <cfg_items.h>

#pragma pack(push, 1)
typedef struct {
    const CFG_HDR cfghdr_0;
    int drg;
    const CFG_CBOX_ITEM cfgcbox_0[3];
    const CFG_HDR cfghdr_1;
    char fmt_str[16];
    const CFG_HDR cfghdr_2;
    int enable_rt_calc;
} CONFIG;
#pragma pack(pop)

extern CONFIG CFG;
extern char CFG_PATH[];

void InitConfig();
