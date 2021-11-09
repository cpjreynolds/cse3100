#ifndef __MATRIX_H
#define __MATRIX_H

#include <assert.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Matrix {
    int r;
    int c;
    int data[]; // NOT [0] like a BARBARIAN. `[]` is in the standard.
} Matrix;

#define M(p, i, j) ((p)->data[(i) * ((p)->c) + (j)])

/*
 * Essentially a sizeof(Matrix) that takes into account the VLA.
 *
 * **NOT** the number of bytes for a rows*cols matrix, but the number of bytes
 * for the entire Matrix struct, fields and data.
 */
inline size_t matrix_sizeof(int r, int c)
{
    assert(r > 0 && c > 0);
    return offsetof(Matrix, data) + r * c * sizeof(int);
}

// My non-infuriating API
Matrix* matrix_make_shared(int rows, int cols);
Matrix* matrix_get_shared(void);

Matrix* matrix_from_file(int fd);
void matrix_print(Matrix* mat);

// Given API. No thanks.
size_t sizeMatrix(int r, int c);
Matrix* makeMatrixMap(void* m, int r, int c);
Matrix* makeMatrix(int r, int c);
Matrix* readMatrix(FILE* fd);
void freeMatrix(Matrix* m);
void printMatrix(Matrix* m);
Matrix* multMatrix(Matrix* a, Matrix* b, Matrix* into);
Matrix* parMultMatrix(int nbW, sem_t* sem, Matrix* a, Matrix* b, Matrix* into);

#endif
