#pragma once

#include <setjmp.h>

typedef struct {
    jmp_buf jmp;
    double *stack;
    int sp;
} DSTACK;

void ConstructDStackStruct(DSTACK* dstack);
void DestructDStackStruct(DSTACK* dstack);
void PushDoubleStack(DSTACK* dstack, double value);
double PopDoubleStack(DSTACK* dstack);
