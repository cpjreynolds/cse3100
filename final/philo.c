#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define MICROSEC 5.0
#define THINK 0
#define HUNGRY 1
#define EAT 2

/* TODO: Complete the ADT Table and functions makeTable, clearTable, muse and main for Q3 */

typedef struct Table {
   /* TODO
    * This ADT should allow for synchronized access to the table by all n philosophers
    * Synchronization should be achieved using mutexes and condition variables
    */ 
} Table;

/* This is the ADT to store information for each philosopher */
typedef struct PhiloTag {
   int pid; //ID
   int state; //activity state
   int ate; //number of times eaten
   int cycle; //number of cycles left
   Table* t; //shared Table ADT
} Philosopher;

Table* makeTable(int n)
{
   Table* t = (Table*)malloc(sizeof(Table));
   /* TODO
    * Initialize an instance of the Table ADT that seats n philosophers
    */
   return t;
}

void clearTable(Table* t)
{
   /* TODO
    * Destroy the shared ADT t and release all resources
    */
}

void doActivity(int activity,Philosopher* p,unsigned* seed)
{
   if (activity == THINK)
      if (p->state != EAT) exit(1);
   if (activity == HUNGRY)
      if (p->state != THINK) exit(2);
   if (activity == EAT)
      if (p->state != HUNGRY) exit(3);
   p->state = activity;
   if (p->state ==  HUNGRY)
      return;
   else {
      char* actName[2] = {"thinking", "eating"};
      double v = ((double)rand_r(seed)) / RAND_MAX * MICROSEC;
      p->ate += p->state == EAT;
      printf("philo [%d] is %s...\n",p->pid, actName[p->state==EAT]);
      usleep(v);
   } 
}

void* muse(Philosopher* p)
{
   unsigned seed = (unsigned)pthread_self(); //seed used to put each philosopher to sleep for a random period of time if the state is thinking or eating
   /* TODO
    * This function is called by each philosopher (thread) p to loop through THINK -> HUNGRY -> EAT -> back to THINK cycles p->cycle number of times. That is, if you ask for p->cycle cycles, you ought to enter the EAT state p->cycle times without locking. 
    * All philosophers are concurrent. A philosopher can only enter the EAT state if they can pick up both the left and right forks. Once eaten, a philosopher must put down the forks for others to use.
    * To enter each of these states, a philosopher calls the doActivity function.
    */
   return NULL;
}

int main(int argc,char* argv[])
{
   int n = atoi(argv[1]); // number of philosophers
   int c = atoi(argv[2]); //number of cycles to be completed by each philosopher
   /* TODO
    * This function sets up the multi-threaded computation of the dining philosopher's problem
    * Each thread calls the function muse to perform the prescribed cycles
    */
   return 0;
}
