#pragma once

typedef enum {
    DEGREES,
    RADIANS,
    GRADS
} DRG;

typedef struct {
    double x;
    double y;
    char fmt[16];
    char drg;
    char auto_recalc;
} CALC_SETTINGS;

void InitSettings();
