#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void initVector(IntVector* vec, int cap)
{
  /*
    TODO: The IntVector structure is already allocated in memory. You need to fill in the fields of it. For this, you'll need to allocate an array for cap integers.
   */
}

void freeVector(IntVector* vec)
{
  /*
    TODO: Free the integer array that the IntVector contains. 
  */
}

void expandVector(IntVector* vec)
{
  vec->capacity *= 2;
  /*
    TODO: Use the realloc function to double the capacity of the vector. This is only one line. 
  */
}

void pushBackVector(IntVector* vec, int number)
{
  /*
    TODO: First, check if the vector size is equal to capacity. If it is, then expand it. Then, make the last element of the vector be number; don't forget to increase the size field.
   */
}
int popBackVector(IntVector* vec)
{
  assert(vec->size > 0);
  vec->size--;
  return vec->numbers[vec->size];
}

void pushFrontVector(IntVector* vec, int number)
{
  /*
    TODO: First, check if the vector size is equal to the capacity. If it is, then expand it. Then, insert an element into the front of the vector. Don't forget to increment the size. 
   */
}

int popFrontVector(IntVector* vec)
{
  assert(vec->size > 0);
  /*
    TODO: Remove and return the number at the head of the vector. Be sure to shift remaining elements over to the left (toward the start of the vector) by one place. 
   */
}

void swap(IntVector* vec, size_t indexOne, size_t indexTwo)
{
  assert(indexOne < vec->size);
  assert(indexTwo < vec->size);
  int temp = vec->numbers[indexOne];
  vec->numbers[indexOne] = vec->numbers[indexTwo];
  vec->numbers[indexTwo] = temp;
}

void sortVector(IntVector* vec)
{
  /*
    TODO: Implement Insertion Sort to sort the vector. The swap function will be useful here. 
   */
}
void printVector(IntVector* vec)
{
  printf("(");
  for(size_t i = 0; i < vec->size; i++){
    printf("%d ", vec->numbers[i]);
  }
  printf(")\n");
}

int lengthVector(IntVector* vec)
{
  return vec->size;
}
