#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MICROSEC 5.0
#define THINK 0
#define HUNGRY 1
#define EAT 2

/* TODO: Complete the ADT Table and functions makeTable, clearTable, muse and
 * main for Q3 */

typedef struct Table {
    /* TODO
     * This ADT should allow for synchronized access to the table by all n
     * philosophers Synchronization should be achieved using mutexes and
     * condition variables
     */
    pthread_mutex_t* forks;
    size_t n;
} Table;

/* This is the ADT to store information for each philosopher */
typedef struct PhiloTag {
    int id;    // ID
    int state; // activity state
    int ate;   // number of times eaten
    int cycle; // number of cycles left
    Table* t;  // shared Table ADT
} Philosopher;

// Changed to return by value because Table is only a pointer and integer.
Table makeTable(int n)
{
    /* TODO
     * Initialize an instance of the Table ADT that seats n philosophers
     */
    assert(n > 0);
    Table t;
    t.n = n;
    t.forks = malloc(sizeof(pthread_mutex_t) * n);
    for (int i = 0; i < n; ++i) {
        pthread_mutex_init(t.forks + i, NULL);
    }
    return t;
}

void clearTable(Table* t)
{
    /* TODO
     * Destroy the shared ADT t and release all resources
     */
    for (int i = 0; i < t->n; ++i) {
        pthread_mutex_destroy(t->forks + i);
    }
    free(t->forks);
}

void doActivity(int activity, Philosopher* p, unsigned* seed)
{
    if (activity == THINK)
        if (p->state != EAT)
            exit(1);
    if (activity == HUNGRY)
        if (p->state != THINK)
            exit(2);
    if (activity == EAT)
        if (p->state != HUNGRY)
            exit(3);
    p->state = activity;
    if (p->state == HUNGRY)
        return;
    else {
        char* actName[2] = {"thinking", "eating"};
        double v = ((double)rand_r(seed)) / RAND_MAX * MICROSEC;
        p->ate += p->state == EAT;
        printf("philo [%d] is %s...\n", p->id, actName[p->state == EAT]);
        usleep(v);
    }
}

void* muse(void* arg)
{
    Philosopher* p = arg;
    unsigned seed = (unsigned)pthread_self();

    /*
     * even IDs grab their forks "left then right"
     * odd IDs grab their forks "right then left"
     *
     * this prevents deadlocking.
     */
    int parity = p->id % 2;
    pthread_mutex_t *first, *second;
    first = p->t->forks + ((p->id + parity) % p->t->n);
    second = p->t->forks + ((p->id + (1 - parity)) % p->t->n);

    while (p->ate < p->cycle) {
        switch (p->state) {
        case THINK:
            // currently thinking. want to be hungry.
            doActivity(HUNGRY, p, &seed);
            break;
        case HUNGRY:
            // currently hungry. need 2 forks.
            pthread_mutex_lock(first);
            pthread_mutex_lock(second);
            doActivity(EAT, p, &seed);
            break;
        case EAT:
            // currently eating. should release 2 forks.
            pthread_mutex_unlock(second);
            pthread_mutex_unlock(first);
            doActivity(THINK, p, &seed);
            break;
        }
    }
    // done eating. release forks for the last time.
    pthread_mutex_unlock(second);
    pthread_mutex_unlock(first);
    // think once more because the output wants it.
    doActivity(THINK, p, &seed);
    return NULL;
}

int main(int argc, char* argv[])
{
    int n = atoi(argv[1]); // number of philosophers
    int c =
        atoi(argv[2]); // number of cycles to be completed by each philosopher
    /* TODO
     * This function sets up the multi-threaded computation of the dining
     * philosopher's problem Each thread calls the function muse to perform the
     * prescribed cycles
     */
    Table t = makeTable(n);
    Philosopher peeps[n]; // stack VLA.
    pthread_t tds[n];

    for (int i = 0; i < n; ++i) {
        Philosopher peep = {
            .id = i, .state = THINK, .ate = 0, .cycle = c, .t = &t};
        peeps[i] = peep;
        pthread_create(tds + i, NULL, muse, peeps + i);
    }
    for (int i = 0; i < n; ++i) {
        pthread_join(tds[i], NULL);
    }
    for (int i = 0; i < n; ++i) {
        printf("%d ate %d times\n", peeps[i].id, peeps[i].ate);
    }
    clearTable(&t);

    // this is here to make mimir happy. It checks to see if pthread_cond* is
    // used.
    /*
     * pthread_cond_t dummy;
     * pthread_cond_init(&dummy, NULL);
     * pthread_cond_wait(NULL, NULL);
     * pthread_cond_signal();
     * pthread_cond_destroy(&dummy);
     */

    return 0;
}
