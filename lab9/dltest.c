#include "dllist.h"
#include <stdio.h>
#include <string.h>

int main(int argc,char* argv[])
{
   int nbT = atoi(argv[1]);
   DLList* lst = malloc(sizeof(DLList));
   initList(lst);

   for(int i=0;i < 100;i++) {
     char k[10];
     sprintf(k, "%d", i);
     char v[10];
     sprintf(v, "%d", i*2);
     insertSortedList(lst, k,v);
   }

   printListForward(lst);
      
   destroyList(lst);
   free(lst);
   return 0;
}

