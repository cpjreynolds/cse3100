#include "hashMap.h"

#include<stdio.h>


typedef struct AddPairsThreadArgs{
  HashMap* hm;
  char** keys;
  char** values;
  unsigned int numPairs;
} AddPairsThreadArgs;

typedef struct RemoveKeysThreadArgs{
  HashMap* hm;
  char** keys;
  unsigned int numKeys;
} RemoveKeysThreadArgs;

void* addPairs(void* arg)
{
  AddPairsThreadArgs* ta = (AddPairsThreadArgs*) arg;
  for(unsigned int i = 0; i < ta->numPairs; i++){
    insertHashMap(ta->hm, ta->keys[i], ta->values[i]);    
  }
  return NULL;
}

void* removeKeys(void* arg)
{
  RemoveKeysThreadArgs* ta = (RemoveKeysThreadArgs*) arg;
  for(unsigned int i = 0; i < ta->numKeys; i++){
    //printf("Key: %s, Value: %s\n", ta->keys[i], retrieveValue(ta->hm, ta->keys[i]));
    //printf("Removing Key %s\n", ta->keys[i]);
    deleteEntry(ta->hm, ta->keys[i]);
    //printf("After removal Key: %s, Value: %s\n", ta->keys[i], retrieveValue(ta->hm, ta->keys[i]));
  }
  return NULL;
}

int main(){
  char* keys[10] = {"jordan", "hello", "hola", "terrence", "one", "two", "three", "four", "five", "six"};
  char* values[10] = {"force", "bye", "adios", "bell", "a", "b", "c", "d", "e", "f"};
  HashMap hm;
  initHashMap(&hm, 3);
  pthread_t threadOne;
  AddPairsThreadArgs one;
  one.hm = &hm;
  one.keys = keys;
  one.values = values;
  one.numPairs = 5;
  pthread_t threadTwo;
  AddPairsThreadArgs two;
  two.hm = &hm;
  two.keys = keys + 5;
  two.values = values + 5;
  two.numPairs = 5;
  pthread_create(&threadOne, NULL, addPairs, &one);
  pthread_create(&threadTwo, NULL, addPairs, &two);
  pthread_join(threadOne, NULL);
  pthread_join(threadTwo, NULL);
  RemoveKeysThreadArgs oneRemove;
  oneRemove.hm = &hm;
  oneRemove.keys = keys;
  oneRemove.numKeys = 5;
  RemoveKeysThreadArgs twoRemove;
  twoRemove.hm = &hm;
  twoRemove.keys = keys + 5;
  twoRemove.numKeys = 5;
  pthread_create(&threadOne, NULL, removeKeys, &oneRemove);
  pthread_create(&threadTwo, NULL, removeKeys, &twoRemove);
  pthread_join(threadOne, NULL);
  pthread_join(threadTwo, NULL);
  destroyHashMap(&hm);
  
  return 0;
}
