#ifndef __VECTOR_H
#define __VECTOR_H

#include <stdio.h>

typedef struct IntVector{
  int* numbers;
  size_t size;
  size_t capacity;
} IntVector;

void initVector(IntVector* vec, int cap);
void freeVector(IntVector* vec);

void pushBackVector(IntVector* vec, int number);
int popBackVector(IntVector* vec);

void pushFrontVector(IntVector* vec, int number);
int popFrontVector(IntVector* vec);

void sortVector(IntVector* vec);
void printVector(IntVector* vec);
int lengthVector(IntVector* vec);
#endif
