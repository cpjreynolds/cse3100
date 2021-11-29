#include "guess-mt.h"
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct thread_arg_tag {
    gmn_t* shared;
    pthread_mutex_t lock;
    pthread_cond_t cvar;
} thread_arg_t;

void* thread_c(void* consumer_thread_data);
void* thread_p(void* producer_thread_data);

int main(int argc, char* argv[])
{
    /* TODO
     * Create an instance of thread_arg_t and initialize it. The value to be
     * guessed is taken as input from the command line arguments. Create two
     * threads - one each for the parent and child Reap the thread resources and
     * destroy the mutexes and condition variables used.
     */
    if (argc != 2) {
        puts("USAGE: pclock <value>");
        exit(EXIT_FAILURE);
    }
    thread_arg_t targs = {0};
    pthread_cond_init(&targs.cvar, NULL);
    pthread_mutex_init(&targs.lock, NULL);
    int val = atoi(argv[1]);
    targs.shared = gmn_init(val);

    pthread_t tc, tp;
    pthread_create(&tp, NULL, thread_p, &targs);
    pthread_create(&tc, NULL, thread_c, &targs);

    pthread_join(tp, NULL);
    pthread_join(tc, NULL);

    free(targs.shared);
    pthread_mutex_destroy(&targs.lock);
    pthread_cond_destroy(&targs.cvar);

    return 0;
}

void* thread_p(void* producer_thread_data)
{
    /* TODO
     * This is a parent thread.
     * repeat the following until guess is correct
         guess a number between min and max. initially max is set to MAX_VALUE
         send the guess to the child and wait for a result (thread_c)
         if result is 0 i.e. you guessed the number correctly, end the thread
         if result is -1 or 1, update the search interval
    */
    thread_arg_t* targs = producer_thread_data;
    int min = 0;
    int max = MAX_VALUE + 1;
    while (1) {
        pthread_mutex_lock(&targs->lock);
        while (targs->shared->status == 1) {
            pthread_cond_wait(&targs->cvar, &targs->lock);
        }
        assert(targs->shared->status == 0);
        puts(targs->shared->message);

        switch (targs->shared->result) {
        case 1: // guess > value
            max = targs->shared->guess;
            break;
        case -1: // guess < value
            min = targs->shared->guess;
            break;
        case 2: // first iteration
            break;
        case 0:         // guess == value
        case 0xbadBeef: // unexpected error
            pthread_mutex_unlock(&targs->lock);
            pthread_exit(NULL);
        }

        targs->shared->guess = (min + max) / 2;
        printf("My guess is %d.\n\n", targs->shared->guess);
        targs->shared->status = 1;
        pthread_cond_signal(&targs->cvar);
        pthread_mutex_unlock(&targs->lock);
    }
    return NULL;
}

void* thread_c(void* consumer_thread_data)
{
    /* TODO
     * This is a child thread.
     * repeat the following until guess is correct
     *      wait for a guess from the parent (thread_p)
     *      call gmn_check() to compare the guess with the user input value
     *      send the result to thread_p
     */
    thread_arg_t* targs = consumer_thread_data;
    int done = 0;
    while (!done) {
        pthread_mutex_lock(&targs->lock);
        while (targs->shared->status == 0) {
            pthread_cond_wait(&targs->cvar, &targs->lock);
        }
        int rv = gmn_check(targs->shared);
        if (rv == 0 || rv == 2 || rv == 0xbadbeef) {
            done = 1;
        }
        assert(targs->shared->status == 1);
        targs->shared->status = 0;
        pthread_cond_signal(&targs->cvar);
        pthread_mutex_unlock(&targs->lock);
    }
    return NULL;
}
