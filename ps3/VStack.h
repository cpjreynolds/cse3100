#ifndef __VSTACK_H
#define __VSTACK_H
#include "vector.h"

typedef struct Stack{
  IntVector vector;
} Stack;

void initStack(Stack* stack);
void pushStack(Stack* stack, int number);
int popStack(Stack* stack);
void freeStack(Stack* stack);
void printStack(Stack* stack);
int isEmpty(Stack* stack);
#endif
