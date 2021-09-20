#include "VStack.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  Stack* stack = malloc(sizeof(Stack));
  initStack(stack);
  int temp;
  int n = 0;
  printf("Enter some numbers: ");
  while(scanf("%d", &temp) == 1){
    pushStack(stack, temp);
    n++;
  }
  printf("Stack: ");
  printStack(stack);
  for(int j = 0; j < n; j++){
    printf("Pop: %d\n", popStack(stack));    
  }
  printf("Stack: ");
  printStack(stack);
  freeStack(stack);
  free(stack);
}
