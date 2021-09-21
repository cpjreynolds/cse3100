#include "VStack.h"
#include "vector.h"

void initStack(Stack* s)
{
    /* TODO: Fill this in. One line of code. */
    initVector(s, 1);
}
void pushStack(Stack* stack, int number)
{
    /* TODO: Fill this in. One line of code. */
    pushBackVector(stack, number);
}
int popStack(Stack* stack)
{
    /* TODO: Fill this in. One line of code. */
    return popBackVector(stack);
}
void freeStack(Stack* stack)
{
    /* TODO: Fill this in. One line of code. */
    freeVector(stack);
}

void printStack(Stack* stack)
{
    /* TODO: Fill this in. One line of code. */
    printVector(stack);
}

int isEmpty(Stack* stack)
{
    /* TODO: Fill this in. One line of code. */
    return !lengthVector(stack);
}
