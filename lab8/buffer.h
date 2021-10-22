#ifndef __BUFFER_H
#define __BUFFER_H

#include <stdlib.h>

typedef struct Task {
   pid_t worker;
   long long   query;
   long long   answer;
} Task;

typedef struct SBuffer {
   size_t sz;
   int enter,leave,nb;
   size_t mapSize;
   Task  data[0];
} SBuffer;

SBuffer* makeBuffer(void* z,int sz,size_t mapSize);
void bufferEnQueue(SBuffer* b,Task t);
Task bufferDeQueue(SBuffer* b);

SBuffer* setupBuffer(char* zone,int nbTasks);
SBuffer* getBuffer(char* zone,int nbTasks);
void tearDownBuffer(char* zone,SBuffer* b);


#endif
