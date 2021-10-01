#include <stdio.h>
#include <stdlib.h>

/* Q5: XOR List This is a simple XOR-List implementation. The idea is
   to store into each node a link that is the XOR of the previous and
   next pointers. Then when you traverse the list, since you know where you are
   coming from, you use that value together with the link to _compute_
   the pointer to the next one (given a travel direction).

   It gives you doubly-linked behavior with the space overhead of singly-linked 
   which is very cool.

   We are providing below convenience data types to get going and they 
   are documented in the handout.
*/

typedef union Ptr {   // This is a convenience type to represent a pointer or a long
   struct XNode* p;   // THis is a 64-bit value
   long          v;   // This is also a 64-bit value
} Ptr;

typedef struct XNode { // DO NOT Modify: this is a list node (value and link pair)
   int  val;
   long link;
} XNode;

typedef struct XList {  // DO NOT Modify: this is the list representation (head,tail) pair
   XNode* head;
   XNode* tail;
} XList;

XList* createList()
{
   // This is given. No need to make any modifications here
   XList* l = (XList*)malloc(sizeof(XList));
   l->head = l->tail = NULL;
   return l;
}

void freeList(XList* l)
{
   // TODO(Q5): deallocate all memory used by the list!
}

void append(XList* l,int v)
{
   // TODO(Q5): add value v at the end of the XORList l
}

void printXList(XNode* from)
{
   // TODO(Q5): print the list starting at location from.
   // From is either the head or the tail of the list
}

void printForward(XList* l)  // That's the beauty. It's the same code in both directions.
{
   printXList(l->head);
}
void printBackward(XList* l) // Yep... same code. simply start at the tail end!
{
   printXList(l->tail);
}

int main()
{
   // We provide the test code for this
   int val;
   XList* l = createList();
   while(scanf("%d",&val) != -1) 
      append(l,val);
   printForward(l);
   printBackward(l);
   freeList(l);
   return 0;
}


