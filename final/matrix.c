#include "matrix.h"
#include <assert.h>
#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

/* TODO: Complete functions detMatrix and detMatrixPar for Q2 */

Matrix* makeMatrix(int r, int c)
{
    Matrix* m = malloc(sizeof(Matrix) + r * c * sizeof(int));
    m->r = r;
    m->c = c;
    return m;
}

int readValue(FILE* fd)
{
    int v = 0;
    char ch;
    int neg = 1;
    while (((ch = getc_unlocked(fd)) != EOF) && isspace(ch))
        ; // skip WS.
    while (!isspace(ch)) {
        if (ch == '-')
            neg = -1;
        else
            v = v * 10 + ch - '0';
        ch = getc_unlocked(fd);
    }
    return neg * v;
}

Matrix* readMatrix(FILE* fd)
{
    int r, c, v;
    int nv = fscanf(fd, "%d %d", &r, &c);
    Matrix* m = makeMatrix(r, c);
    flockfile(fd);
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++) {
            v = readValue(fd);
            M(m, i, j) = v;
        }
    funlockfile(fd);
    return m;
}

Matrix* loadMatrix(char* fName)
{
    FILE* fd = fopen(fName, "r");
    if (fd == NULL)
        return NULL;
    Matrix* m = readMatrix(fd);
    fclose(fd);
    return m;
}

void freeMatrix(Matrix* m)
{
    free(m);
}

void printMatrix(Matrix* m)
{
    for (int i = 0; i < m->r; i++) {
        for (int j = 0; j < m->c; j++)
            printf("%3d ", M(m, i, j));
        printf("\n");
    }
}

Matrix* make_submatrix(Matrix* m, int excl)
{
    assert(m->r == m->c);
    Matrix* new = makeMatrix(m->r - 1, m->c - 1);

    int* dst = new->data;
    for (int row = 1; row < m->r; ++row) {
        int* src = m->data + (row * m->c);
        int* src2 = src + excl + 1;
        size_t len1 = excl;
        size_t len2 = m->c - excl - 1;

        memcpy(dst, src, len1 * sizeof(int));
        memcpy(dst + len1, src2, len2 * sizeof(int));
        dst += new->c;
    }
    return new;
}

int detMatrix(Matrix* m)
{
    /* TODO
     * This function calculates the determinant of matrix m sequentially.
     * Hint: you may create an auxiliary function for ease of implementation.
     * Note that while the indexing of the matrix starts with 0, the inductive
     * definition in the pdf handout starts with a 1. The function returns the
     * determinant of matrix m.
     */
    assert(m->c == m->r);
    if (m->c == 0) {
        return 1;
    }
    else if (m->c == 1) {
        return m->data[0];
    }
    else if (m->c == 2) {
        int* p = m->data;
        return p[0] * p[3] - p[1] * p[2];
    }
    int total = 0;
    for (int col = 0; col < m->c; ++col) {
        Matrix* sub = make_submatrix(m, col);
        int sign = col % 2 == 0 ? 1 : -1;
        total += sign * m->data[col] * detMatrix(sub);
        freeMatrix(sub);
    }
    return total;
}

typedef struct thread_info {
    Matrix* m;
    int start;
    int nterms;
} tinfo_t;

void* matrix_det_thread(void* arg)
{
    tinfo_t* ti = arg;

    long total = 0;
    for (int i = 0; i < ti->nterms; ++i) {
        int col = ti->start + i;
        Matrix* sub = make_submatrix(ti->m, col);
        int sign = col % 2 == 0 ? 1 : -1;
        total += sign * ti->m->data[col] * detMatrix(sub);
        freeMatrix(sub);
    }
    return (void*)total; // return the total by-value as a (void*).
}

int detMatrixPar(Matrix* m, int nbW)
{
    /* TODO
     * This function calculates the determinant of matrix m using nbW threads.
     * Notice that the computation consists of n terms (equal to number of rows
     * or columns). You may split the computation of n terms fairly among the
     * nbW worker threads, and aggregate their results upon completion. The
     * function returns the determinant of matrix m. Note: do not forget the
     * base case for a 0x0 matrix.
     */
    assert(m->c == m->r);
    pthread_t workers[nbW]; // VLA on stack.

    // division of labour
    tinfo_t* tinfos = calloc(nbW, sizeof(tinfo_t));
    tinfos[0].m = m;
    tinfos[0].start = 0;
    tinfos[0].nterms = (m->c / nbW) + (m->c % nbW);
    for (int i = 1; i < nbW; ++i) {
        tinfos[i].m = m;
        tinfos[i].start = tinfos[i - 1].start + tinfos[i - 1].nterms;
        tinfos[i].nterms = m->c / nbW;
    }

    for (int i = 0; i < nbW; ++i) {
        pthread_create(&workers[i], 0, &matrix_det_thread, tinfos + i);
    }
    // type is long instead of int so that typecasting fuckery can work safely
    // sizeof(long) == sizeof(void*) on all platforms that matter for this test
    long result = 0;
    // actual return value still needs to be int though.
    int retval = 0;
    for (int i = 0; i < nbW; ++i) {
        pthread_join(workers[i], (void*)&result);
        retval += result;
    }
    free(tinfos);
    return retval;
}
