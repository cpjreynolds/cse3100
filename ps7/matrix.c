#include "matrix.h"

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <semaphore.h>
#include <stddef.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * Allocates shared memory object for an r x c Matrix.
 *
 * To support multiple matrix programs running simultaneously, the name of the
 * shared memory object is derived from current PID.
 *
 * Child processes created with `fork` will find the shm object by calling
 * `getppid()`
 *
 * This must be called from the parent process.
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

Matrix* matrix_get_shared(void)
{
    char namebuf[NAME_MAX];
    pid_t ppd = getppid();
    snprintf(namebuf, sizeof(namebuf), "/matrix-%jd", (intmax_t)ppd);

    int shmd = shm_open(namebuf, O_RDWR, 0666);
    if (shmd < 0)
        goto error;

    // instead of asking for nrows & ncols, just find the size directly.
    struct stat shmstat;
    int rc = fstat(shmd, &shmstat);
    if (rc != 0)
        goto error;

    Matrix* mat = mmap(NULL, shmstat.st_size, PROT_READ | PROT_WRITE,
                       MAP_SHARED, shmd, 0);
    if (mat == MAP_FAILED)
        goto error;

    // sanity check.
    assert(matrix_sizeof(mat->r, mat->c) == shmstat.st_size);

    close(shmd);
    return mat;
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
 * Unmaps the shared memory from the calling process' address space.
 *
 * Does **NOT** unlink the shared memory object (actually destroying it).
 *
 * Should be called from child processes.
 *
 * Probably not neccessary as mmaps are unmapped on process termination.
 */
void matrix_free_shared(Matrix* mat)
{
    assert(munmap(mat->data, matrix_sizeof(mat->r, mat->c)) == 0);
}

/*
 * Unmaps the shared memory and unlinks (destroys) the named shared memory
 * object.
 *
 * This should only be called from the original parent process.
 */
void matrix_destroy_shared(Matrix* mat)
{
    matrix_free_shared(mat);
    char namebuf[NAME_MAX];
    pid_t pd = getpid();
    snprintf(namebuf, sizeof(namebuf), "/matrix-%jd", (intmax_t)pd);
    assert(shm_unlink(namebuf) == 0);
}

/*
 * I wanna go fast.
 */
Matrix* matrix_from_file(int fd)
{
    struct stat fs;
    int rc = fstat(fd, &fs);
    if (rc != 0)
        goto error;
    char* fmap = mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    // strtol sets the second argument to point to the character following the
    // last character interpreted as the integer.
    char* start = fmap;
    char* end;
    int rows = strtol(start, &end, 0);
    assert(start != end);
    int cols = strtol(end, &start, 0);
    assert(start != end);

    Matrix* mat = matrix_make(rows, cols);

    for (int i = 0; i < rows * cols; i++) {
        int v = strtol(start, &end, 0);
        assert(start != end);
        mat->data[i] = v;
        start = end;
    }

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
 * returns the number of bytes needed to hold a matrix of r x c values
 * (type int)
 */
size_t sizeMatrix(int r, int c)
{
    assert(0); // stub.
    return 0;
}

/*
 * allocates space to hold a matrix of r x c values (type int)
 */
Matrix* makeMatrix(int r, int c)
{
    assert(0);
    return NULL;
}

/*
 * Allocates space for a matrix of r x c from a _given_ memory block at
 * address zone.
 */
Matrix* makeMatrixMap(void* zone, int r, int c)
{
    assert(0); // stub
    return NULL;
}

/*
 * Reads a single integer from the FILE fd
 * output: a single integer.
 * Note: the function uses getc_unlocked to _not_ pay the overhead of locking
 * and unlocking the file for each integer to be read (imagine reading a matrix
 * of 1000x1000, that's 1,000,000 calls to getc and therefore 1,000,000 calls to
 * locking the file.
 */
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
    /*
     * Read a matrix from a file (fd). The matrix is held in a text format that
     * first conveys the number of rows and columns, then, each row is on a line
     * of text and all the values are separated by white spaces. Namely: r c
     * v0,0 v0,1 .... v0,c-1
     * v1,0 v1,1 .... v1,c-1
     * ....
     * vr-1,0 ....    v_r-1,c-1
     */
    int r, c, v;
    int nv = fscanf(fd, "%d %d", &r, &c);
    assert(nv == 2);
    Matrix* m = makeMatrix(r, c);
    flockfile(fd);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            v = readValue(fd);
            M(m, i, j) = v;
        }
    }
    funlockfile(fd);
    return m;
}

void freeMatrix(Matrix* m)
{
    /*
     * deallocates the space used by matrix m
     */
}

void printMatrix(Matrix* m)
{
    /*
     * Print the matrix on the sandard output. One row per line, values for the
     * row separated by white spaces.
     */
}

Matrix* multMatrix(Matrix* a, Matrix* b, Matrix* into)
{ // mxn * nxp yields an mxp matrix
    /*
     * Compute the product of A * B and store the result in `into`.
     * The computation is sequential and is only meant to be used as a
     * check for your parallel code.
     *
     * Return value: the matrix `into` were the result is held.
     */
}

Matrix* parMultMatrix(int nbW, sem_t* sem, Matrix* a, Matrix* b, Matrix* into)
{
    /*
     * Compute the produce of A * B and store the result in `into`
     * The compuration is done in parallel with nbW worker **processes** (NOT
     * threads). It should have the same output as the sequential version, but
     * faster. ;-) How you use the semaphore is up to you (as long as you use
     * it!) You CANNOT use the wait system call.
     *
     * Return value: the matrix `into` were the result is held.
     */
}
