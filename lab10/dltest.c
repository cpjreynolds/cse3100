#include "dllist.h"
#include <stdio.h>
#include <string.h>

typedef struct Job {
    DLList* theList;
    int nbToInsert;
} Job;

void* insertData(void* theJobPtr)
{
    Job* job = theJobPtr;
    for (int i = 0; i < job->nbToInsert; i++) {
        insertSortedList(job->theList, i);
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    int nbT = atoi(argv[1]);
    DLList* lst = malloc(sizeof(DLList));
    initList(lst);
    Job theJob = {lst, 100};
    pthread_t tid[nbT];
    for (int k = 0; k < nbT; k++)
        pthread_create(tid + k, NULL, insertData, &theJob);

    for (int k = 0; k < nbT; k++)
        pthread_join(tid[k], NULL);

    printListForward(theJob.theList);

    destroyList(lst);
    free(lst);
    return 0;
}
