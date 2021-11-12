#ifndef __HASHMAP_H
#define __HASHMAP_H

#include <pthread.h>
#include "dllist.h"
typedef struct Bucket {
  DLList* list;
  pthread_mutex_t lock;
} Bucket;

typedef struct HashMap {
  unsigned int numBuckets;
  unsigned int numItems;
  Bucket* buckets;
  pthread_mutex_t tableLock;
} HashMap;


void initHashMap(HashMap* hm, unsigned int nb);
void clearHashMap(HashMap* hm);
void destroyHashMap(HashMap* hm);
void insertHashMap(HashMap* hm, char* key, char* value);
char* retrieveValue(HashMap* hm, char* key);
void deleteEntry(HashMap* hm, char* key);



#endif
