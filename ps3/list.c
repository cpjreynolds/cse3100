#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Returns the penultimate node or null if there was only one.
 */
static inline ILNode* il_penult(ILNode* node)
{
    ILNode* prev = NULL;
    while (node && node->next) {
        prev = node;
        node = node->next;
    }
    return prev;
}

void initList(IntList* list)
{
    memset(list, 0, sizeof(IntList));
}

void freeList(IntList* list)
{
    ILNode* n;
    while (list->head) {
        n = list->head->next;
        free(list->head);
        list->head = n;
    }
    list->len = 0;
}

// Add a node containing the number to the end of the list
void pushBackList(IntList* list, int num)
{
    list->len++;
    // should handle len==1 properly.
    if (list->tail) {
        assert(!list->tail->next);
        list->tail->next = calloc(1, sizeof(ILNode));
        list->tail = list->tail->next;
        list->tail->number = num;
        return;
    }
    // empty list.
    assert(list->head == list->tail && list->tail == NULL);
    list->head = list->tail = calloc(1, sizeof(ILNode));
    list->head->number = num;
}

// Remove the tail node of the list, and return the integer contained in it
int popBackList(IntList* list)
{
    assert(list->head && list->len > 0);
    if (list->head == list->tail) {
        assert(list->len == 1);
        int rc = list->head->number;
        free(list->head);
        memset(list, 0, sizeof(IntList));
        return rc;
    }
    ILNode* p = il_penult(list->head);
    assert(p->next == list->tail);
    assert(!list->tail->next);

    int rc = list->tail->number;
    free(list->tail);
    list->tail = p;
    list->tail->next = NULL;
    list->len--;
    return rc;
}

/*
 * Add a node containing the number to the start of the list
 */
void pushFrontList(IntList* list, int num)
{
    ILNode* n = malloc(sizeof(ILNode));
    n->next = list->head;
    n->number = num;
    list->head = n;
    if (!list->tail) {
        list->tail = list->head;
    }
    list->len++;
}

/*
 * Remove the head node of the list, and return the integer contained
 * in it.
 */
int popFrontList(IntList* list)
{
    assert(list->head && list->len > 0);
    ILNode* n = list->head->next;
    int rc = list->head->number;
    free(list->head);
    list->head = n;
    if (!list->head) {
        list->tail = list->head;
    }
    list->len--;
    return rc;
}

static void sortedInsert(IntList* slist, ILNode* new)
{
    /*
      TODO: You're given a linked list where the nodes are sorted (the smallest
      number is at the head, the largest at the tail). Iterate through the list
      until you find where to insert nodeToInsert, so as to keep the list
      sorted, and insert it.
     */

    if (slist->head == NULL) {
        slist->head = new;
        slist->head->next = NULL;
        slist->len = 1;
        slist->tail = slist->head;
        return;
    }

    if (new->number <= slist->head->number) {
        new->next = slist->head;
        slist->head = new;
        slist->len++;
        return;
    }

    ILNode* cursor = slist->head;
    while (cursor->next && new->number > cursor->next->number)
        cursor = cursor->next;
    ILNode* rest = cursor->next;
    cursor->next = new;
    new->next = rest;
    slist->len++;
    if (!rest)
        slist->tail = new;
}

void sortList(IntList* list)
{
    IntList newList = {NULL, NULL};
    ILNode* next = NULL;
    ILNode* cur = list->head;
    while (cur) {
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
