#include "dllist.h"
#include <stdio.h>
#include <string.h>
void initList(DLList* list)
{
   list->head = list->tail = NULL;
}
void clearList(DLList* list)
{
   DLNode* cur = list->head;
   while (cur) {
      DLNode* next = cur->next;
      free(cur->key);
      free(cur->value);
      free(cur);
      cur = next;
   }
   list->head = list->tail = NULL;
}
void destroyList(DLList* list)
{
   clearList(list);
}

char* getValue(DLList* list, char* key)
{
  DLNode* cur = list->head;
  while(cur && strcmp(cur->key, key) < 0)
    cur = cur->next;
  if(cur && strcmp(cur->key, key) == 0)
    return cur->value;
  else
    return NULL;
}

int insertSortedList(DLList* list, char* key, char* val)
{
   DLNode* cur = list->head;
   while (cur && strcmp(cur->key, key) < 0)
      cur = cur->next;
   if(cur && strcmp(cur->key, key) == 0)
     return 0;
   DLNode* nn = malloc(sizeof(DLNode));
   nn->value = (char*) malloc(sizeof(char)*(1 + strlen(val)));
   nn->key = (char*) malloc(sizeof(char)*(1 + strlen(key)));
   strcpy(nn->value, val);
   strcpy(nn->key, key);
   if (cur) {
      // We found the spot: before `cur`
      DLNode* prev = cur->prv;
      DLNode* nxt  = cur;
      
      nn->prv = prev;
      nn->next = cur;
      if (prev) prev->next = nn;
      else list->head = nn;
      nxt->prv = nn;
   } else {
      nn->next = NULL;
      nn->prv = list->tail;
      if (list->tail)
         list->tail->next = nn;
      else list->head = nn;
      list->tail = nn;
   }
   return 1;
}


int removeFromList(DLList* list, char* key)
{
   DLNode* cur = list->head;
   while (cur && strcmp(cur->key, key) < 0)
      cur = cur->next;
   if (cur) {
      if (cur->prv)
         cur->prv->next = cur->next;
      else list->head = cur->next;
      if (cur->next)
         cur->next->prv = cur->prv;
      else list->tail = cur->prv;
      free(cur->key);
      free(cur->value);
      free(cur);
      return 1;
   }
   return 0;
}

void printListForward(DLList* list)
{
   DLNode* cur = list->head;
   while (cur) {
     printf("(%s, %s) ",cur->key, cur->value);
     cur = cur->next;
   }
   printf("\n");
}
