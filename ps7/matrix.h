#ifndef __MATRIX_H
#define __MATRIX_H

#include <assert.h>
#include <inttypes.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Matrix {
    int r;
    int c;
    int data[]; // NOT [0] like a BARBARIAN. `[]` is in the C99 standard.
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

// My Much More Better-er API.
Matrix* matrix_make_shared(int rows, int cols);
void matrix_delete_shared(Matrix* mat);

Matrix* matrix_make(int rows, int cols);
void matrix_free(Matrix* mat);

Matrix* matrix_from_file(int fd);
void matrix_print(Matrix* mat);

Matrix* matrix_mult(Matrix* a, Matrix* b, Matrix* into);
Matrix* matrix_mult_par(int nworkers, sem_t* sem, Matrix* a, Matrix* b,
                        Matrix* into);

/*
 * stubs for the Less Than Spectacular(TM) API we were given.
 *
 * Yes, I really am this stubborn and childish.
 *
 * I'd be lying if I said I was sorry though.
 */

inline size_t sizeMatrix(int r, int c)
{
    return matrix_sizeof(r, c);
}

inline Matrix* makeMatrixMap(void* m, int r, int c)
{
    // Imma just pretend I didn't see this memory you gave me.
    (void)m;
    // And give you some of that good shit.
    return matrix_make_shared(r, c);
}

inline Matrix* makeMatrix(int r, int c)
{
    return matrix_make(r, c);
}

inline Matrix* readMatrix(FILE* fd)
{
    int fn = fileno(fd);
    return matrix_from_file(fn);
}

inline void freeMatrix(Matrix* m)
{
    matrix_free(m);
}

inline void printMatrix(Matrix* m)
{
    matrix_print(m);
}

inline Matrix* multMatrix(Matrix* a, Matrix* b, Matrix* into)
{
    return matrix_mult(a, b, into);
}

inline Matrix* parMultMatrix(int nbW, sem_t* sem, Matrix* a, Matrix* b,
                             Matrix* into)
{
    return matrix_mult_par(nbW, sem, a, b, into);
}

#endif
