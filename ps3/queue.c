#include "queue.h"
#include "LStack.h"
#include "vector.h"
#include <stdio.h>

void initQueue(Queue* q)
{
    /* TODO: Initialize the stacks of the queue */
    initStack(&q->one);
    initStack(&q->two);
}
void enQueue(Queue* q, int number)
{
    /* TODO: Fill this in, one line of code */
    pushStack(&q->one, number);
}
int deQueue(Queue* q)
{
    /*
      TODO: Check if stack 2 is empty. If it is, flip all of the elements from
      stack 1 to stack 2. Then, pop and return the element from stack 2.
     */
    if (isEmpty(&q->two)) {
        while (!isEmpty(&q->one))
            pushStack(&q->two, popStack(&q->one));
    }
    return popStack(&q->two);
}
void freeQueue(Queue* q)
{
    freeStack(&q->one);
    freeStack(&q->two);
}
