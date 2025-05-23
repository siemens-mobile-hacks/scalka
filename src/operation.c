#include <swilib.h>
#include <stdlib.h>
#include <math.h>
#include "stack.h"
#include "settings.h"
#include "operation.h"
#include "calc_object.h"

#define PI_CONST 3.1415926535897932384626433832795

double D_ANSWER;
int OP_LEN, OP_POS;
char OPERATION[256];

__attribute__((always_inline))
inline double athh(double a) {
    return log((1 + a) / (1 - a)) / 2;
}

double ConvertRadiansToAngle(double radian) {
    double a = 0;
    switch (SETTINGS.drg) {
      case DEGREES: default:
        a = radian * 180 / PI_CONST;
      break;
      case RADIANS:
        a = radian;
      break;
      case GRADS:
        a = radian * 200 / PI_CONST;
      break;
    }
    return a;
}

double ConvertAngleToRadians(double angle) {
    double a = 0;
    switch (SETTINGS.drg) {
      default: case DEGREES:
        a = angle * PI_CONST / 180;
      break;
      case RADIANS:
        a = angle;
      break;
      case GRADS:
        a = angle * PI_CONST / 200;
      break;
    }
    return a;
}

void ParseOperation(DSTACK *dstack, int operation) {
    double a, b, ans;
    switch (operation) {
      case OP_SIN: // sin
        a = PopDoubleStack(dstack);
        ans = sin(ConvertAngleToRadians(a));
        PushDoubleStack(dstack, ans);
      break;
      case OP_MULT: // *
        a = PopDoubleStack(dstack);
        b = PopDoubleStack(dstack);
        ans = b * a;
        PushDoubleStack(dstack, ans);
      break;
      case OP_DIV: // /
        a = PopDoubleStack(dstack);
        b = PopDoubleStack(dstack);
        ans = b / a;
        PushDoubleStack(dstack, ans);
      break;
      case OP_COS: // cos
        a = PopDoubleStack(dstack);
        ans = cos(ConvertAngleToRadians(a));
        PushDoubleStack(dstack, ans);
      break;
      case OP_PLUS: // +
        a = PopDoubleStack(dstack);
        b = PopDoubleStack(dstack);
        ans = b + a;
        PushDoubleStack(dstack, ans);
      break;
      case OP_MINUS: // -
        a = PopDoubleStack(dstack);
        b = PopDoubleStack(dstack);
        ans = b - a;
        PushDoubleStack(dstack, ans);
      break;
      case OP_TAN: // tan
        a = PopDoubleStack(dstack);
        ans = tan(ConvertAngleToRadians(a));
        PushDoubleStack(dstack, ans);
      break;
      case OP_SQUARE: // ^2
        a = PopDoubleStack(dstack);
        ans = pow(a, 2);
        PushDoubleStack(dstack, ans);
      break;
      case OP_SQRT: // sqrt
        a = PopDoubleStack(dstack);
        ans = sqrt(a);
        PushDoubleStack(dstack, ans);
      break;
      case OP_ASIN: // asin
        a = PopDoubleStack(dstack);
        ans = ConvertRadiansToAngle(asin(a));
        PushDoubleStack(dstack, ans);
      break;
      case OP_LN: // ln
        a = PopDoubleStack(dstack);
        ans = log(a);
        PushDoubleStack(dstack, ans);
      break;
      case OP_LOG: // log 10
        a = PopDoubleStack(dstack);
        ans = log10(a);
        PushDoubleStack(dstack, ans);
      break;
      case OP_ACOS: // acos
        a = PopDoubleStack(dstack);
        ans = ConvertRadiansToAngle(acos(a));
        PushDoubleStack(dstack, ans);
      break;
      case OP_EPOW: // e^
        a = PopDoubleStack(dstack);
        ans = exp(a);
        PushDoubleStack(dstack, ans);
      break;
      case OP_ATAN: // atan
        a = PopDoubleStack(dstack);
        ans = ConvertRadiansToAngle(atan(a));
        PushDoubleStack(dstack, ans);
      break;
      case OP_TENPOW: // 10^
        a = PopDoubleStack(dstack);
        ans = pow(10, a);
        PushDoubleStack(dstack, ans);
      break;
      case OP_POW: // ^
        a = PopDoubleStack(dstack);
        b = PopDoubleStack(dstack);
        ans = pow(b, a);
        PushDoubleStack(dstack, ans);
      break;
      case OP_SH: // sinh
        a = PopDoubleStack(dstack);
        ans = sinh(ConvertAngleToRadians(a));
        PushDoubleStack(dstack, ans);
      break;
      case OP_ASH: // asinh
        a = PopDoubleStack(dstack);
        ans = ConvertRadiansToAngle(asinh(a));
        PushDoubleStack(dstack, ans);
      break;
      case OP_CH: // cosh
        a = PopDoubleStack(dstack);
        ans = cosh(ConvertAngleToRadians(a));
        PushDoubleStack(dstack, ans);
      break;
      case OP_ACH: // acosh
        a = PopDoubleStack(dstack);
        ans = ConvertRadiansToAngle(acosh(a));
        PushDoubleStack(dstack, ans);
      break;
      case OP_TH: // tanh
        a = PopDoubleStack(dstack);
        ans = tanh(ConvertAngleToRadians(a));
        PushDoubleStack(dstack, ans);
      break;
      case OP_ATH: // atanh
        a = PopDoubleStack(dstack);
        ans = ConvertRadiansToAngle(athh(a));
        PushDoubleStack(dstack, ans);
      break;
      case OP_ABS: // abs
        a = PopDoubleStack(dstack);
        ans = fabs(a);
        PushDoubleStack(dstack, ans);
      break;
      case OP_FAC: // !
        a = PopDoubleStack(dstack);
        ans = gamma(a + 1);
        PushDoubleStack(dstack, ans);
      break;
  }
}

__attribute__((always_inline))
inline int IsCharNumber(int c) {
    return ((c >= DIG_1 && c <= DIG_0) || c == OP_NEG);
}

__attribute__((always_inline))
inline int GetCharByIndex(int c) {
    return (c == 29 ? '-' : c == 9 ? '.' : c == 10 ? '0' : c + '1');
}

__attribute__((always_inline))
inline int IsMathFunc(int c) {
    return (c == OP_SIN || c == OP_COS || c == OP_TAN || c == OP_ASIN || c == OP_LN || c == OP_LOG || c == OP_ACOS ||
            c == OP_ATAN || c == OP_SH || c == OP_ASH || c == OP_CH || c == OP_ACH || c == OP_ABS || c == OP_TH ||
            c == OP_ATH || c == OP_FAC);
}

int IsPowFunc(int c) {
    return (c == OP_SQUARE || c == OP_SQRT || c == OP_EPOW || c == OP_TENPOW || c == OP_POW);
}

/* Функция PRIOR возвpащает пpиоpитет аpифм. опеpации */
int GetPriority(int a) {
    if (IsPowFunc(a)) {
      return 5;
    }
    if (IsMathFunc(a)) {
      return 4;
    }
    switch (a) {
      case OP_MULT: // *
      case OP_DIV: // /
        return 3;
      case OP_PLUS: // +
      case OP_MINUS: // -
        return 2;
      case LEFTBRACKET: // (
        return 1;
      default:
        return 0;
    }
}

void insert_operation(int op) {
    if (OP_LEN < sizeof(OPERATION)) {
        for (int i = OP_LEN; i >= OP_POS; i--) {
            OPERATION[i + 1] = OPERATION[i];
        }
        OPERATION[OP_POS++] = op;
        OP_LEN++;
    }
}

void remove_operation() {
    if (OP_LEN && OP_POS) {
        if (OP_POS <= OP_LEN) {
            int i = OP_POS;
            while (i < OP_LEN) {
                OPERATION[i - 1] = OPERATION[i];
                i++;
            }
            OP_POS--;
            OP_LEN--;
        }
    }
}

void remove_all_operation() {
    int i = 0;
    while (i <= OP_LEN) {
        OPERATION[i] = 0;
        i++;
    }
    OP_POS = 0;
    OP_LEN = 0;
}

void calc_answer() {
    char *s;
    int c;
    int i = OP_LEN;
    char stack[256];
    int stack_depth = 0;

    s = OPERATION;
    char *d, value[128];
    double ans = 0;

    DSTACK dstack;
    ConstructDStackStruct(&dstack);
    if (setjmp(dstack.jmp)) {
      ans = NAN;
      goto L_ERROR;
    }
    while (i) {
      c = *s++;
      if (IsCharNumber(c)) {
        d = value;
        do {
          *d++ = GetCharByIndex(c);
          i--;
          c = *s++;
        }
        while (IsCharNumber(c) && i);
        *d = 0;
        double a = strtod(value, NULL);
        PushDoubleStack(&dstack, a);
        if (!i) break;
      }
      if (c == NUM_PI) { // пи
        double a = PI_CONST;
        PushDoubleStack(&dstack, a);
      } else if (c == NUM_ANS) { // ANS
          PushDoubleStack(&dstack, D_ANSWER);
      } else if (c == NUM_A) { // X
          PushDoubleStack(&dstack, SETTINGS.x);
      } else if (c == NUM_B) { // Y
          PushDoubleStack(&dstack, SETTINGS.y);
      } else if (c >= VAR_A && c <= VAR_Z) {
          static double vars['z' - 'a'];
          PushDoubleStack(&dstack, vars[c - VAR_A]);
      } else if (c == RIGHTBRACKET) {// Закрывающаяся скобка
          while (stack_depth && stack[--stack_depth] != LEFTBRACKET) {
            ParseOperation(&dstack, stack[stack_depth]);
          }
      } else if (c == LEFTBRACKET) { // Открывающаяся скобка
          stack[stack_depth++] = c;
      } else if (c == OP_MULT || c == OP_DIV || c == OP_PLUS || c == OP_MINUS || IsMathFunc(c) || IsPowFunc(c)) { // Если знак операции
          if (!stack_depth) { // Если стек пуст
            stack[stack_depth++] = c;
          } else if (GetPriority(c) > GetPriority(stack[stack_depth - 1])) { // Если приоритет поступившей операции больше того что на вершине стека
            stack[stack_depth++] = c;
          } else {
            while (stack_depth && (GetPriority(stack[stack_depth - 1]) >= GetPriority(c))) {
              ParseOperation(&dstack, stack[--stack_depth]);
            }
            stack[stack_depth++] = c;
          }
      }
      i--;
    }
    while (stack_depth) {
      ParseOperation(&dstack, stack[--stack_depth]);
    }
    ans = PopDoubleStack(&dstack);
    L_ERROR:
        DestructDStackStruct(&dstack);
        D_ANSWER = ans;
}

int GetOperationIndexByKey(int key) {
    if (key >= '1' && key <= '9') {
        return key - '1';
    } else if (key == '0') {
        return 10;
    } else if (key == '*') {
        return 9;
    }
    return 11;
}
