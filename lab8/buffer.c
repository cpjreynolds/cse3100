#include "buffer.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <unistd.h>

SBuffer* setupBuffer(char* zone, size_t ntasks)
{
    // I fucking dare someone to tell me this could be better without goto.

    int shmd = shm_open(zone, O_RDWR | O_CREAT, 0666);
    if (shmd < 0)
        goto error;

    size_t nbytes = offsetof(SBuffer, data) + ntasks * sizeof(Task);
    int rv = ftruncate(shmd, nbytes);
    if (rv)
        goto error;

    SBuffer* sb =
        mmap(NULL, nbytes, PROT_READ | PROT_WRITE, MAP_SHARED, shmd, 0);
    if (sb == MAP_FAILED)
        goto error;

    sb->cap = ntasks;
    sb->head = sb->tail = 0;
    sb->mapsz = nbytes;

    return sb;
error:
    perror("setupBuffer:");
    exit(EXIT_FAILURE);
}

// Given a named memory mapping zone (capable of holding nbTasks), return
// the address of the shared buffer held at the beginning of that memory
// map.
SBuffer* getBuffer(char* zone, size_t ntasks)
{
    int shmd = shm_open(zone, O_RDWR, 0666);
    if (shmd < 0)
        goto error;

    size_t nbytes = offsetof(SBuffer, data) + ntasks * sizeof(Task);

    SBuffer* sb =
        mmap(NULL, nbytes, PROT_READ | PROT_WRITE, MAP_SHARED, shmd, 0);
    if (sb == MAP_FAILED)
        goto error;

    return sb;
error:
    perror("getBuffer:");
    exit(EXIT_FAILURE);
}

// Given a memory mapping at address z, meant to hold sz Task for a total
// size (in bytes) of mapSize initialize and return a shared buffer pointer
// held in that mapping.
SBuffer* makeBuffer(void* z, size_t sz, size_t mapSize)
{

    // Kinda feels like this is extraneous garbage I don't need to actually
    // implement.
}

// Release the resources (map/shared object) needed for the shared buffer b
void tearDownBuffer(char* zone, SBuffer* b)
{
    // first unmap, then unlink.
    // releasing resources shouldn't fail, so assert instead of checking
    assert(munmap(b, b->mapsz) == 0);
    assert(shm_unlink(zone) == 0);
}

// add Task t to the shared buffer b.
void bufferEnQueue(SBuffer* b, Task t)
{
    // if (__sync_bool_compare_and_swap(&b->head, b->cap, 0)) {

    // b->data[0] = t;
    //}
    // else {
    // size_t idx = __sync_fetch_and_add(&b->head, 1);
    // b->data[idx] = t;
    //}
    size_t idx = b->head;
    b->data[idx] = t;
    b->head = (b->head + 1) % b->cap;
}

// deQueue and return a task from the shared buffer b.
Task bufferDeQueue(SBuffer* b)
{
    // if (__sync_bool_compare_and_swap(&b->tail, b->cap, 0)) {
    // return b->data[0];
    //}
    // else {
    // size_t idx = __sync_fetch_and_add(&b->tail, 1);
    // return b->data[idx];
    //}
    size_t idx = b->tail;
    Task t = b->data[idx];
    b->tail = (b->tail + 1) % b->cap;
    return t;
}
