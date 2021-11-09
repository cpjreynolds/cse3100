#include "matrix.h"
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <unistd.h>

/* Convenience function to load a matrix from a file */
Matrix* loadMatrix(char* fName)
{
    FILE* fd = fopen(fName, "r");
    if (fd == NULL)
        return NULL;
    Matrix* m = readMatrix(fd);
    fclose(fd);
    return m;
}

// A better function.
Matrix* matrix_load(const char* fname)
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
    matrix_print(a);
    matrix_print(b);

    // TODO

    // Carry out the computation and print the resulting matrix.

    return 0;
}
