#include "matrix.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <unistd.h>

// A better function.
static Matrix* matrix_load(const char* fname)
{
    int fd = open(fname, O_RDONLY);
    if (fd < 0)
        return NULL;
    Matrix* mat = matrix_from_file(fd);
    close(fd);
    return mat;
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        printf("usage: mult <file1> <file2> <#workers>\n");
        exit(1);
    }
    Matrix* a = matrix_load(argv[1]);
    Matrix* b = matrix_load(argv[2]);
    int nworkers = atoi(argv[3]);
    if (a == NULL || b == NULL) {
        printf("Wrong filenames. Couldn't load matrices [%s,%s]\n", argv[1],
               argv[2]);
        exit(2);
    }

    // TODO
    // Carry out the computation and print the resulting matrix.

    char sem_namebuf[NAME_MAX];
    snprintf(sem_namebuf, sizeof(sem_namebuf), "/sem-%jd", (intmax_t)getpid());
    sem_t* sem = sem_open(sem_namebuf, O_CREAT | O_EXCL, S_IRWXU, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    Matrix* result = matrix_make_shared(a->r, b->c);
    // do the thing.
    matrix_mult_par(nworkers, sem, a, b, result);

    // each worker `sem_post()`s once after completion.
    for (int i = 0; i < nworkers; i++) {
        sem_wait(sem);
    }

    matrix_print(result);

    matrix_delete_shared(result);
    sem_close(sem);
    sem_unlink(sem_namebuf);
    return 0;
}
