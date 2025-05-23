#include <swilib.h>
#include <stdlib.h>
#include "stack.h"

void ConstructDStackStruct(DSTACK* dstack) {
    dstack->stack = 0;
    dstack->sp = 0;
}

void DestructDStackStruct(DSTACK* dstack) {
    mfree(dstack->stack);
}

void PushDoubleStack(DSTACK* dstack, double value) {
    dstack->stack = realloc(dstack->stack, (dstack->sp + 1) * sizeof(double)); // NOLINT
    dstack->stack[dstack->sp] = value;
    dstack->sp++;
}

double PopDoubleStack(DSTACK* dstack) {
    double a = 0;
    if (dstack->sp) {
        a = dstack->stack[--dstack->sp];
    }
    else {
        longjmp(dstack->jmp, 1);
    }
    return a;
}
