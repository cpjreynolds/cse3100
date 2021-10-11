#include <stdio.h>
#include <stdlib.h>

void my_error(char* s)
{
    // print our own error message to standard error output
    fprintf(stderr, "Error:%s\n", s);
    // Use perror() to print the descriptive message about the error
    perror("errno:");
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    int n;
    unsigned int seed;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <seed> <n> <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // not checking the value of seed and n
    seed = atoi(argv[1]);
    n = atoi(argv[2]);
    srand(seed);

    // try to open the file specified on the command line
    FILE* fp = fopen(argv[3], "w");

    // check the return values!!
    if (fp == NULL)
        my_error("fopen() returned NULL.");

    for (int i = 0; i < n; i++) {
        int r = rand(); // get a pseudo-random value

        // write the integer to the file and check the return value!!
        if (fwrite(&r, sizeof(r), 1, fp) != 1)
            my_error("fwrite() failed");
    }
    fclose(fp);
    return 0;
}
