#ifndef __BUFFER_H
#define __BUFFER_H

#include <stdlib.h>

typedef struct Task {
    pid_t worker;
    long long query;
    long long answer;
} Task;

// the original struct fields were hot garbage & confusing so I changed them.
typedef struct SBuffer {
    size_t cap;           // maximum number of tasks in the buffer.
    volatile size_t head; // offset to add next task at
    volatile size_t tail; // offset to take next task from
    size_t mapsz;         // size of the shared memory map in bytes
    Task data[]; // Flexible array members are part of the C99 standard.
                 // NOT data[0] like a barbarian.
} SBuffer;

SBuffer* makeBuffer(void* z, size_t cap, size_t mapSize);
void bufferEnQueue(SBuffer* b, Task t);
Task bufferDeQueue(SBuffer* b);

SBuffer* setupBuffer(char* zone, size_t nbTasks);
SBuffer* getBuffer(char* zone, size_t nbTasks);
void tearDownBuffer(char* zone, SBuffer* b);

#endif
