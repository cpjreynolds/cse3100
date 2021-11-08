#ifndef __MATRIX_H
#define __MATRIX_H

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Matrix {
    int r, c;
    int data[]; // NOT [0] like a BARBARIAN. `[]` is in the standard.
} Matrix;

#define M(p, i, j) ((p)->data[(i) * ((p)->c) + (j)])

size_t sizeMatrix(int r, int c);
Matrix* makeMatrixMap(void* m, int r, int c);
Matrix* makeMatrix(int r, int c);
Matrix* readMatrix(FILE* fd);
void freeMatrix(Matrix* m);
void printMatrix(Matrix* m);
Matrix* multMatrix(Matrix* a, Matrix* b, Matrix* into);
Matrix* parMultMatrix(int nbW, sem_t* sem, Matrix* a, Matrix* b, Matrix* into);

#endif
