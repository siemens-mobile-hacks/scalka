#pragma once

extern double D_ANSWER;
extern int OP_LEN, OP_POS;
extern char OPERATION[256];

void insert_operation(int op);
void remove_operation();
void remove_all_operation();
void calc_answer();

int GetOperationIndexByKey(int key);
