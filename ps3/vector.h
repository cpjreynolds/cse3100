#ifndef __VECTOR_H
#define __VECTOR_H

#include <stdio.h>

typedef struct IntVector {
    int* buf;
    size_t len;
    size_t cap;
} IntVector;

void initVector(IntVector* vec, size_t cap);
void freeVector(IntVector* vec);

void pushBackVector(IntVector* vec, int number);
int popBackVector(IntVector* vec);

void pushFrontVector(IntVector* vec, int number);
int popFrontVector(IntVector* vec);

void sortVector(IntVector* vec);
void printVector(IntVector* vec);
int lengthVector(IntVector* vec);
#endif
