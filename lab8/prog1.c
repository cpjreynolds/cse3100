#include "buffer.h"
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <unistd.h>

// yeah yeah globals are bad. sue me.
static pid_t workers[256] = {0};

// it isn't stupid if it works.
static void remember_worker(pid_t w)
{
    int i;
    for (i = 0; workers[i] != 0; i++) {
        if (workers[i] == w) {
            return;
        }
    }
    workers[i] = w;
}

static void killallworkers(void)
{
    for (int i = 0; workers[i] != 0; i++) {
        kill(workers[i], SIGTERM);
    }
}

int main()
{
    SBuffer* requests = setupBuffer("/requests", 100000);
    SBuffer* answers = setupBuffer("/answers", 200);
    printf("Number of answer slots:%ld\n", answers->cap);
    sem_t* slots = sem_open("/semaSlots", O_CREAT, S_IRWXU, answers->cap);
    sem_t* tasks = sem_open("/semaTasks", O_CREAT, S_IRWXU, 0);
    sem_t* asw = sem_open("/semaASW", O_CREAT, S_IRWXU, 0);
    sem_t* lock = sem_open("/semaPullLock", O_CREAT, S_IRWXU, 1);
    sem_t* lock2 = sem_open("semaOtherlock", O_CREAT, S_IRWXU, 1);
    for (int i = 0; i < requests->cap; i++) {
        Task t = {0, i, -1};
        sem_wait(lock2);
        bufferEnQueue(requests, t);
        sem_post(lock2);
        sem_post(tasks);
    }
    long long check = 0;
    for (int i = 0; i < requests->cap; i++) {
        sem_wait(asw);
        sem_wait(lock);
        Task t = bufferDeQueue(answers);
        sem_post(lock);
        sem_post(slots);
        // printf("Worker produced (%d,%lld,%lld)\n", t.worker, t.query,
        // t.answer);
        check += t.answer;
        remember_worker(t.worker);
    }
    printf("Final: %lld\n", check);
    tearDownBuffer("/requests", requests);
    tearDownBuffer("/answers", answers);
    sem_close(lock2);
    sem_close(lock);
    sem_close(tasks);
    sem_close(slots);
    sem_close(asw);
    killallworkers();
    sem_unlink("/semaSlots");
    sem_unlink("/semaTasks");
    sem_unlink("/semaASW");
    sem_unlink("/semaPullLock");
    return 0;
}
