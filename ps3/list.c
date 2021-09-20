#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


void initList(IntList* list)
{
  /*
    TODO: The IntList structure is already allocated in memory. You need to fill the fields in. 
   */
}
void freeList(IntList* list)
{
  /*
    TODO: You need to free each node in the list. However, the actual list itself doesn't need to be freed here.
   */
}

void pushBackList(IntList* list, int number)
{
  /*
    TODO: Add a node containing the number to the end of the list
   */
}
int popBackList(IntList* list)
{
  assert(list->head && list->tail && list->len > 0);
    /*
    TODO: Remove the tail node of the list, and return the integer contained in it.
  */
}
void pushFrontList(IntList* list, int number)
{
  /*
    TODO: Add a node containing the number to the start of the list 
  */
}
int popFrontList(IntList* list)
{
  assert(list->head && list->tail && list->len > 0);
  /*
    TODO: Remove the head node of the list, and return the integer contained in it. 
  */
}

void sortedInsert(IntList* sortedList, ILNode* nodeToInsert){
  /*
    TODO: You're given a linked list where the nodes are sorted (the smallest number is at the head, the largest at the tail). Iterate through the list until you find where to insert nodeToInsert, so as to keep the list sorted, and insert it. 
   */
}

void sortList(IntList* list)
{
  IntList newList = {NULL,NULL};
  ILNode* next = NULL;
  ILNode* cur = list->head;
  while(cur){
    next = cur->next;
    sortedInsert(&newList, cur);
    cur = next;
  }
  *list = newList;
}



int lengthList(IntList* list)
{
  return list->len;
}
void printList(IntList* list)
{
   printf("(");
   ILNode* node = list->head;
   while (node) {
      printf("%d ", node->number);
      node = node->next;
   }
   printf(")\n");
}
