#ifndef __DLLIST_H
#define __DLLIST_H

#include <stdlib.h>

typedef struct DLNode {
    struct DLNode* next;
    struct DLNode* prv;
    char* key;
    char* value;
} DLNode;

typedef struct DLList {
    DLNode* head;
    DLNode* tail;
} DLList;

void initList(DLList* list);
void clearList(DLList* list);
void destroyList(DLList* list);
int insertSortedList(DLList* list, char* key, char* val);
char* getValue(DLList* list, char* key);
int removeFromList(DLList* list, char* key);
void printListForward(DLList* list);

#endif
