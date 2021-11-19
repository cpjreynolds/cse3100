#ifndef __DLLIST_H
#define __DLLIST_H

#include <pthread.h>
#include <stdlib.h>

typedef struct DLNode {
    struct DLNode* next;
    struct DLNode* prv;
    int value;
    pthread_mutex_t mtx;
} DLNode;

typedef struct DLList {
    DLNode* head;
    DLNode* tail;
    pthread_mutex_t mtx;
} DLList;

void initList(DLList* list);
void clearList(DLList* list);
void destroyList(DLList* list);
void insertSortedList(DLList* list, int v);
void printListForward(DLList* list);

#endif
