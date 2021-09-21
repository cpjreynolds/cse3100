#include "LStack.h"
#include "list.h"

void initStack(Stack* s)
{
    /* TODO: Fill this in. One line of code. */
    initList(s);
}

void pushStack(Stack* stack, int number)
{
    /* TODO: Fill this in. One line of code. */
    pushBackList(stack, number);
}

int popStack(Stack* stack)
{
    /* TODO: Fill this in. One line of code. */
    return popBackList(stack);
}

void freeStack(Stack* stack)
{
    /* TODO: Fill this in. One line of code. */
    freeList(stack);
}

void printStack(Stack* stack)
{
    /* TODO: Fill this in. One line of code. */
    printList(stack);
}

int isEmpty(Stack* stack)
{
    /* TODO: Fill this in. One line of code. */
    return !lengthList(stack);
}
