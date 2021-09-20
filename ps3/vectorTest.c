#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
  IntVector* vec = malloc(sizeof(IntVector));
  initVector(vec, 2);
  printVector(vec);
  printf("Pushing 2 to back: ");
  pushBackVector(vec, 2);
  printVector(vec);
  printf("Pushing 1 to back: ");
  pushBackVector(vec, 1);
  printVector(vec);
  printf("Pushing 20 to front: ");
  pushFrontVector(vec, 20);
  printVector(vec);
  printf("Pushing 25 to front: ");
  pushFrontVector(vec, 25);
  printVector(vec);
  printf("Pushing 8 to back: ");
  pushBackVector(vec, 8);
  printVector(vec);
  printf("Pushing 3 to back: ");
  pushBackVector(vec, 3);
  printVector(vec);
  printf("Pushing 10 to back: ");
  pushBackVector(vec, 10);  
  printVector(vec);
  printf("Sorting vector: ");
  sortVector(vec);
  printVector(vec);
  int num = popFrontVector(vec);
  printf("Pop front %d: ", num);
  printVector(vec);
  num = popBackVector(vec);
  printf("Pop back %d: ", num);
  printVector(vec);
  freeVector(vec);
  
}
