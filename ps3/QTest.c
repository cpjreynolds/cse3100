#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  Queue* q = malloc(sizeof(Queue));
  initQueue(q);
  int temp;
  int n = 0;
  printf("Enter some numbers: ");
  while(scanf("%d", &temp) == 1){
    enQueue(q, temp);
    n++;
  }
  for(int j = 0; j < n; j++){
    printf("Dequeue: %d\n", deQueue(q));    
  }
  freeQueue(q);
  free(q);
}
