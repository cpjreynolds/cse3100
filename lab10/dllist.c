#include "dllist.h"
#include <assert.h>
#include <stdio.h>

void initList(DLList* list)
{
    list->head = list->tail = NULL;
    pthread_mutex_init(&list->mtx, NULL);
}
void clearList(DLList* list)
{
    DLNode* cur = list->head;
    while (cur) {
        DLNode* next = cur->next;
        pthread_mutex_destroy(&cur->mtx);
        free(cur);
        cur = next;
    }
    list->head = list->tail = NULL;
}
void destroyList(DLList* list)
{
    clearList(list);
    pthread_mutex_destroy(&list->mtx);
}

void insertSortedList(DLList* list, int v)
{
    /*
      TODO: Iterate through the list, find the correct place to insert the
      integer v. Each node has a mutex; use those mutexes to coordinate the
      threads.
     */

    pthread_mutex_t* curlock = &list->mtx;
    pthread_mutex_lock(curlock);

    DLNode* cur = list->head;
    while (cur && cur->value <= v) {
        /*
         * if (!cur->next) {
         *     break;
         * }
         */
        pthread_mutex_lock(&cur->mtx);
        pthread_mutex_unlock(curlock);
        curlock = &cur->mtx;
        cur = cur->next;
    }
    DLNode* nn = malloc(sizeof(DLNode));
    nn->value = v;
    pthread_mutex_init(&nn->mtx, NULL);
    if (cur) {
        // We found the spot: before `cur`
        DLNode* prev = cur->prv;
        DLNode* nxt = cur;

        nn->prv = prev;
        nn->next = cur;
        if (prev) {
            prev->next = nn;
        }
        else {
            list->head = nn;
        }
        nxt->prv = nn;
    }
    else {
        nn->next = NULL;
        nn->prv = list->tail;
        if (list->tail) {
            list->tail->next = nn;
        }
        else {
            list->head = nn;
        }
        list->tail = nn;
    }
    pthread_mutex_unlock(curlock);
}

void printListForward(DLList* list)
{
    pthread_mutex_lock(&list->mtx);
    DLNode* cur = list->head;
    pthread_mutex_t* last = &list->mtx;
    while (cur) {
        pthread_mutex_lock(&cur->mtx);
        printf("%d ", cur->value);
        pthread_mutex_unlock(last);
        last = &cur->mtx;
        cur = cur->next;
    }
    printf("\n");
    pthread_mutex_unlock(last);
}
