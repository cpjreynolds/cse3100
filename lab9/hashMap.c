#include "hashMap.h"
#include "dllist.h"
#include <string.h>

unsigned long long hash(char* key)
{
  const int p = 31;
  const int m = 1e9 + 9;
  unsigned long long hash_value = 0;
  long long p_pow = 1;
  for (int i = 0; i < strlen(key); i++) {
    char c = key[i];
    hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
    p_pow = (p_pow * p) % m;
  }
  return hash_value;
}
void initHashMap(HashMap* hm, unsigned int nb)
{
  /*
    TODO: Set the numBuckets and numItems fields of hm.
    allocate memory for nb buckets, allocate memory for a DLList for each bucket, initialize the list, and initialize the bucket's lock. 

    Initialize the tableLock.

   */
}
void clearHashMap(HashMap* hm)
{
  /*
    TODO: free up the hash map and buckets, destroy the mutexes. Set the numBuckets and numItems fields to zero. 
   */
}
void destroyHashMap(HashMap* hm)
{
  clearHashMap(hm);
}

void insertHashMap(HashMap* hm, char* key, char* value)
{
  
  unsigned int h = hash(key) % hm->numBuckets;
  /*
    TODO: Go to bucket h, and insert the (key, value) pair, unless the key is already in the hash map. If you inserted the pair, increment the hash map's numItems field (under mutex protection, of course!)

   */
}
char* retrieveValue(HashMap* hm, char* key)
{
  unsigned int h = hash(key) % hm->numBuckets;
  /*
    TODO: Find the key in bucket h, under mutex protection. Since the bucket's key-value pairs are stored in a DLList, look at the DLList.h file for an appropriate function to get the value corresponding to the key.
   */
}
void deleteEntry(HashMap* hm, char* key)
{
  unsigned int h = hash(key) % hm->numBuckets;
  /*
    TODO: Delete the key from bucket h, under mutex protection. If they key isn't in the bucket, don't do anything. If the key was successfully removed from the bucket, decrement the hashmap's numItems field. 
   */
}

