#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <math.h>

/* TODO: Complete the main and createSamples functions for Q1 */

long createSamples(unsigned wID,long nbSamples,double d)
{
   long ttl = 0; // Number of events detected by a process
   unsigned seed = wID; //seed for random number generation

   /* TODO
    * This function is called by all processes to perform nbSamples number of trials
    * You may use rand_r() from stdlib.h to generate random numbers between 0 and RAND_MAX. Seed value for rand_r() is given above.
    * The usage of rand_r() can be found in the manual page: man -S2 rand.
    * The random numbers can be normalized to an interval [0, 1) by dividing them by RAND_MAX. 
    * The function returns the number of events occurred in nbSamples trials.
    */

   return ttl;
}

int main(int argc,char* argv[])
{
   if (argc < 4) {
      printf("usage: rpair <d> <samples> <workers>\n");
      exit(1);
   }
   float d = atof(argv[1]); 
   long s  = atol(argv[2]); // total number of trials to be executed
   int nbW = atoi(argv[3]); // number of worker processes

   long ttl = 0; //total number of events
   double p = 0; //Probability of the occurence of events

   /* TODO
    * This is the parent process that sets up the computation involving shared maps and multiple processes.
    * The computation must be fairly distributed among nbW worker (child) processes.
    * The parent must aggregate the results from all worker processes, report the final result, release all resources.
    */

   printf("Total trials = %ld \t Total events = %ld \t Probability = %lf\n",s,ttl,p);
   return 0;
}
