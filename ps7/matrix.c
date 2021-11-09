#include "matrix.h"

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <semaphore.h>
#include <stddef.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * Allocates shared memory object for an r x c Matrix.
 *
 * To support multiple matrix programs running simultaneously, the name of the
 * shared memory object is derived from current PID.
 *
 * Due to the implementation, each instance of the `matrix` program can create
 * at most one shared matrix object. This will always be the result matrix.
 */
Matrix* matrix_make_shared(int rows, int cols)
{
    char namebuf[NAME_MAX];

    pid_t pd = getpid();
    // MISRA C recommends casts to intmax_t/uintmax_t for integer formatting.
    snprintf(namebuf, sizeof(namebuf), "/matrix-%jd", (intmax_t)pd);

    int shmd = shm_open(namebuf, O_RDWR | O_CREAT | O_EXCL, 0666);
    if (shmd < 0)
        goto error;

    size_t nbytes = matrix_sizeof(rows, cols);
    int rc = ftruncate(shmd, nbytes);
    if (rc)
        goto error;

    Matrix* mat =
        mmap(NULL, nbytes, PROT_READ | PROT_WRITE, MAP_SHARED, shmd, 0);
    if (mat == MAP_FAILED)
        goto error;

    close(shmd); // Don't need the fd once it has been mapped.
    mat->r = rows;
    mat->c = cols;
    return mat;
error:
    perror(__func__);
    exit(EXIT_FAILURE);
}

/*
 * Unmaps the shared memory and unlinks the named shared memory
 * object.
 *
 * This should only be called from the parent process.
 */
void matrix_delete_shared(Matrix* mat)
{
    int rc = munmap(mat, matrix_sizeof(mat->r, mat->c));
    if (rc)
        goto error;

    char namebuf[NAME_MAX];
    snprintf(namebuf, sizeof(namebuf), "/matrix-%jd", (intmax_t)getpid());
    rc = shm_unlink(namebuf);
    if (rc)
        goto error;
    return;
error:
    perror(__func__);
    exit(EXIT_FAILURE);
}

Matrix* matrix_make(int rows, int cols)
{
    size_t nbytes = matrix_sizeof(rows, cols);
    Matrix* mat = malloc(nbytes);
    mat->r = rows;
    mat->c = cols;
    return mat;
}

void matrix_free(Matrix* mat)
{
    free(mat->data);
}

/*
 * I wanna go fast.
 */
Matrix* matrix_from_file(int fd)
{
    struct stat fs;
    int rc = fstat(fd, &fs);
    if (rc)
        goto error;
    char* fmap = mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (fmap == MAP_FAILED)
        goto error;

    // strtol sets the second argument to point to the character following the
    // last character interpreted as the integer.
    char* start = fmap;
    char* end;
    int rows = strtol(start, &end, 0);
    assert(start != end);
    int cols = strtol(end, &start, 0);
    assert(start != end);

    Matrix* mat = matrix_make(rows, cols);

    // get_rekt_laurent.jpg
    for (int i = 0; i < rows * cols; i++) {
        int v = strtol(start, &end, 0);
        assert(start != end); // sanity check
        mat->data[i] = v;
        start = end;
    }

    rc = munmap(fmap, fs.st_size);
    if (rc)
        goto error;
    return mat;
error:
    perror(__func__);
    exit(EXIT_FAILURE);
}

void matrix_print(Matrix* mat)
{
    for (int r = 0; r < mat->r; r++) {
        for (int c = 0; c < mat->c - 1; c++) {
            printf("%d ", M(mat, r, c));
        }
        printf("%d\n", M(mat, r, mat->c - 1));
    }
}

/*
 * Computes the dot product of the row `a` and column `b`. len is the width of
 * `a`, stride is the width of `b`.
 */
static inline int dot_prod(int* a, int* b, int len, int stride)
{
    int acc = 0;
    for (int i = 0; i < len; i++) {
        acc += *a * *b;
        a++;
        b += stride;
    }
    return acc;
}

/*
 * Compute the product of A * B and store the result in `res`.
 * The computation is sequential and is only meant to be used as a
 * check for your parallel code.
 *
 * mxn * nxp yields an mxp matrix
 *
 * Return value: the matrix `res` were the result is held.
 */
Matrix* matrix_mult(Matrix* a, Matrix* b, Matrix* res)
{
    assert(a->c == b->r);
    for (int r = 0; r < a->r; r++) {
        for (int c = 0; c < b->c; c++) {
            int* row_a = a->data + (a->c * r);
            int* col_b = b->data + c;
            M(res, r, c) = dot_prod(row_a, col_b, a->c, b->c);
        }
    }
    return res;
}

/*
 * computes the partial (`nrows`) matrix product of A*B, starting at `startrow`.
 *
 * This is the child process entry and exit point.
 */
static void do_partial_mult(Matrix*, Matrix*, Matrix*, sem_t*, int, int)
    // optimise me harder daddy.
    __attribute__((noreturn));

static void do_partial_mult(Matrix* a, Matrix* b, Matrix* res, sem_t* sem,
                            int nrows, int startrow)
{
    int* resptr = res->data + (startrow * res->c);
    for (int r = 0; r < nrows; r++) {
        for (int c = 0; c < b->c; c++) {
            int* row_a = a->data + (a->c * startrow) + (a->c * r);
            int* col_b = b->data + c;
            *resptr = dot_prod(row_a, col_b, a->c, b->c);
            resptr++;
        }
    }
    // signal we're done.
    sem_post(sem);
    // need to exit the child process, or we'll return to the dispatch loop and
    // fuck shit up.
    exit(0);
}

/*
 * Compute the product of A * B and store the result in `res`
 * The compuration is done in parallel with nbW worker **processes** (NOT
 * threads). It should have the same output as the sequential version, but
 * faster. ;-) How you use the semaphore is up to you (as long as you use
 * it!) You CANNOT use the wait system call.
 *
 * Return value: the matrix `res` were the result is held.
 */
Matrix* matrix_mult_par(int nworkers, sem_t* sem, Matrix* a, Matrix* b,
                        Matrix* res)
{
    assert(a->c == b->r);
    // distribute rows as equally as division allows.
    int worker_rows[nworkers];
    int rows_each = a->r / nworkers;
    for (int i = 0; i < nworkers; i++) {
        // memset writes bytes, not ints, hence this loop.
        worker_rows[i] = rows_each;
    }
    // then add remaining rows.
    for (int i = 0; i < a->r % nworkers; i++) {
        worker_rows[i]++;
    }

    int startrow = 0;
    for (int i = 0; i < nworkers; i++) {
        if (fork() == 0) {
            do_partial_mult(a, b, res, sem, worker_rows[i], startrow);
        }
        startrow += worker_rows[i];
    }

    // once all workers have posted to the semaphore, the result matrix is ready
    // to be read from.
    return res;
}
