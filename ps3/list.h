#ifndef __LIST_H
#define __LIST_H
#include <stdlib.h>

typedef struct ILNode{
  struct ILNode* next;
  int number;
} ILNode;

typedef struct IntList{
  ILNode* head;
  ILNode* tail;
  size_t len;
} IntList;

void initList(IntList* list);
void freeList(IntList* list);

void pushBackList(IntList* list, int number);
int popBackList(IntList* list);
void pushFrontList(IntList* list, int number);
int popFrontList(IntList* list);

void sortList(IntList* list);
int lengthList(IntList* list);
void printList(IntList* list);

#endif
