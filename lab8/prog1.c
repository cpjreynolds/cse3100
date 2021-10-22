#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "buffer.h"

int main()
{
   SBuffer* requests = setupBuffer("/requests",100000);
   SBuffer* answers = setupBuffer("/answers",200);
   printf("Number of answer slots:%ld\n",answers->sz);
   sem_t* slots = sem_open("/semaSlots",O_CREAT,S_IRWXU,answers->sz);
   sem_t* tasks = sem_open("/semaTasks",O_CREAT,S_IRWXU,0);
   sem_t* asw = sem_open("/semaASW",O_CREAT,S_IRWXU,0);
   sem_t* lock  = sem_open("/semaPullLock",O_CREAT,S_IRWXU,1);
   for(int i=0;i< requests->sz;i++) {
      Task t = {0,i,-1};
      bufferEnQueue(requests,t);
      sem_post(tasks);
   }
   long long check = 0;
   for(int i=0;i< requests->sz;i++) {
      sem_wait(asw);
      sem_wait(lock);
      Task t = bufferDeQueue(answers);
      sem_post(lock);
      sem_post(slots);
      //printf("Worker produced (%d,%lld,%lld)\n",t.worker,t.query,t.answer);
      check += t.answer;
   }
   printf("Final: %lld\n",check);
   tearDownBuffer("/requests",requests);
   tearDownBuffer("/answers",answers);
   sem_close(lock);
   sem_close(tasks);
   sem_close(slots);
   sem_close(asw);
   return 0;
}
