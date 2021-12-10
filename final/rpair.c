#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* TODO: Complete the main and createSamples functions for Q1 */

static void createSamples(unsigned, double, long*) __attribute__((noreturn));

void createSamples(unsigned wID, double d, long* shared)
{
    long ttl = 0; // Number of events detected by a process
    // unsigned seed = wID; // seed for random number generation // why.

    /* TODO
     * This function is called by all processes to perform nbSamples number of
     * trials You may use rand_r() from stdlib.h to generate random numbers
     * between 0 and RAND_MAX. Seed value for rand_r() is given above. The usage
     * of rand_r() can be found in the manual page: man -S2 rand. The random
     * numbers can be normalized to an interval [0, 1) by dividing them by
     * RAND_MAX. The function returns the number of events occurred in nbSamples
     * trials.
     */
    long nbSamples = *shared;
    while (nbSamples) {
        // Just hoping integer promotion rules don't fuck me sideways here.
        double x, y;
        x = rand_r(&wID);
        y = rand_r(&wID);
        x /= RAND_MAX;
        y /= RAND_MAX;
        if (fabs(x - y) < d) {
            ++ttl;
        }
        --nbSamples;
    }
    *shared = ttl;
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        printf("usage: rpair <d> <samples> <workers>\n");
        exit(1);
    }
    float d = atof(argv[1]);
    long trials = atol(argv[2]);  // total number of trials to be executed
    int nworkers = atoi(argv[3]); // number of worker processes

    int shmd = shm_open("/rpair-shm", O_CREAT | O_RDWR | O_EXCL, 0666);
    if (shmd < 0)
        goto error;

    size_t shmlen = sizeof(long) * nworkers;
    if (ftruncate(shmd, shmlen))
        goto error;

    long* shared =
        mmap(NULL, shmlen, PROT_READ | PROT_WRITE, MAP_SHARED, shmd, 0);
    if (shared == MAP_FAILED)
        goto error;
    close(shmd);

    // distribute workload
    for (int i = 0; i < nworkers; ++i) {
        shared[i] = trials / nworkers;
    }
    shared[0] += trials % nworkers;

    for (int wid = 0; wid < nworkers; ++wid) {
        if (fork() == 0) {
            createSamples(wid, d, &shared[wid]);
        }
    }
    // once this returns, all results are available in shared buffer.
    for (int i = 0; i < nworkers; ++i) {
        wait(NULL);
    }

    long ttl = 0; // total number of events
    double p = 0; // Probability of the occurence of events

    for (int i = 0; i < nworkers; ++i) {
        ttl += shared[i];
    }
    p = (double)ttl / (double)trials;

    printf("Total trials = %ld \t Total events = %ld \t Probability = %lf\n",
           trials, ttl, p);
    munmap(shared, shmlen);
    shm_unlink("/rpair-shm");
    return 0;
error:
    perror(__func__);
    shm_unlink("/rpair-shm");
    return 1;
}
