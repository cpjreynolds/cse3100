#ifndef __LSTACK_H
#define __LSTACK_H
#include "list.h"

typedef struct Stack{
  IntList list; 
} Stack;

void initStack(Stack* stack);
void pushStack(Stack* stack, int number);
int popStack(Stack* stack);
void freeStack(Stack* stack);
void printStack(Stack* stack);
int isEmpty(Stack* stack);
#endif
