#include "buffer.h"
#include <assert.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <unistd.h>

void solve(Task* t)
{
    long long ttl = 0;
    for (int i = 0; i <= t->query; i++)
        ttl += i;
    t->answer = ttl;
    t->worker = getpid();
}

sem_t *slots, *tasks, *asw, *lock, *lock2;

void term_handler(int sn)
{
    sem_close(slots);
    sem_close(tasks);
    sem_close(asw);
    sem_close(lock);
    sem_close(lock2);
    exit(0);
}

int main()
{
    struct sigaction ac = {0};
    ac.sa_handler = term_handler;
    sigaction(SIGTERM, &ac, NULL);

    SBuffer* requests = getBuffer("/requests", 100000);
    SBuffer* answers = getBuffer("/answers", 200);
    slots = sem_open("/semaSlots", O_EXCL);
    tasks = sem_open("/semaTasks", O_EXCL);
    asw = sem_open("/semaASW", O_EXCL);
    lock = sem_open("/semaPullLock", O_EXCL);
    lock2 = sem_open("/semaOtherlock", O_EXCL);
    // Do the rest!

    while (1) {
        sem_wait(tasks);
        sem_wait(lock2);
        Task t = bufferDeQueue(requests);
        sem_post(lock2);
        solve(&t);

        sem_wait(slots);
        sem_wait(lock);
        bufferEnQueue(answers, t);
        sem_post(lock);
        sem_post(asw);
    }

    assert(0);
}
