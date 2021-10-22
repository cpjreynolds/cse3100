#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include "buffer.h"

void solve(Task* t)
{
   long long ttl = 0;
   for(int i=0;i <= t->query;i++)
      ttl += i;
   t->answer = ttl;
   t->worker = getpid();
}

int main()
{
   SBuffer* requests = getBuffer("/requests",100000);
   SBuffer* answers  = getBuffer("/answers",200);
   sem_t* slots   = sem_open("/semaSlots",O_EXCL);
   sem_t* tasks   = sem_open("/semaTasks",O_EXCL);
   sem_t* asw     = sem_open("/semaASW",O_EXCL);
   sem_t* lock    = sem_open("/semaPullLock",O_EXCL);
   // Do the rest!
   
   // We give you the deallocation
   sem_close(slots);
   sem_close(tasks);
   sem_close(asw);
   sem_close(lock);
   return 0;
}
