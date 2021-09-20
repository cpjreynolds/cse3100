#ifndef __QUEUE_H
#define __QUEUE_H
#include "LStack.h"
typedef struct Queue{
  Stack* one;
  Stack* two;
} Queue;

void initQueue(Queue* q);
void enQueue(Queue* q, int number);
int deQueue(Queue* q);
void freeQueue(Queue* q);


#endif
