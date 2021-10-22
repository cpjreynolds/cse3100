#include "buffer.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <unistd.h>

SBuffer* setupBuffer(char* zone, int nbTasks)
{
    // Create and initialize a memory mapping in the zone named "zone" capable
    // of holding nbTasks. return the address of the shared buffer.
}

SBuffer* makeBuffer(void* z, int sz, size_t mapSize)
{
    // Given a memory mapping at address z, meant to hold sz Task for a total
    // size (in bytes) of mapSize initialize and return a shared buffer pointer
    // held in that mapping.
}

SBuffer* getBuffer(char* zone, int nbTasks)
{
    // Given a named memory mapping zone (capable of holding nbTasks), return
    // the address of the shared buffer held at the beginning of that memory
    // map.
}

void tearDownBuffer(char* zone, SBuffer* b)
{
    // Release the resources (map/shared object) needed for the shared buffer b
}

void bufferEnQueue(SBuffer* b, Task t)
{
    // add Task t to the shared buffer b.
}

Task bufferDeQueue(SBuffer* b)
{
    // deQueue and return a task from the shared buffer b.
}
