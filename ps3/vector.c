#include "vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initVector(IntVector* vec, size_t cap)
{
    /*
      TODO: The IntVector structure is already allocated in memory. You need to
      fill in the fields of it. For this, you'll need to allocate an array for
      cap integers.
     */
    vec->buf = malloc(sizeof(int) * cap);

    vec->len = 0;
    vec->cap = cap;
}

void freeVector(IntVector* vec)
{
    free(vec->buf);
    memset(vec, 0, sizeof(IntVector));
}

void expandVector(IntVector* vec)
{
    vec->cap *= 2;
    /*
      TODO: Use the realloc function to double the capacity of the vector. This
      is only one line.
    */
    vec->buf = realloc(vec->buf, vec->cap);
}

void pushBackVector(IntVector* vec, int number)
{
    /*
      TODO: First, check if the vector size is equal to capacity. If it is, then
      expand it. Then, make the last element of the vector be number; don't
      forget to increase the size field.
     */
    if (vec->len == vec->cap)
        expandVector(vec);
    vec->buf[vec->len] = number;
    vec->len++;
}
int popBackVector(IntVector* vec)
{
    assert(vec->len > 0);
    vec->len--;
    return vec->buf[vec->len];
}

void pushFrontVector(IntVector* vec, int number)
{
    /*
      TODO: First, check if the vector size is equal to the capacity. If it is,
      then expand it. Then, insert an element into the front of the vector.
      Don't forget to increment the size.
     */
    if (vec->len == vec->cap)
        expandVector(vec);
    memmove(vec->buf + 1, vec->buf, vec->len * sizeof(int));
    vec->buf[0] = number;
    vec->len++;
}

int popFrontVector(IntVector* vec)
{
    assert(vec->len > 0);
    /*
      TODO: Remove and return the number at the head of the vector. Be sure to
      shift remaining elements over to the left (toward the start of the vector)
      by one place.
     */
    int rc = vec->buf[0];
    memmove(vec->buf, vec->buf + 1, vec->len * sizeof(int));
    vec->len--;
    return rc;
}

void swap(IntVector* vec, size_t indexOne, size_t indexTwo)
{
    assert(indexOne < vec->len);
    assert(indexTwo < vec->len);
    int temp = vec->buf[indexOne];
    vec->buf[indexOne] = vec->buf[indexTwo];
    vec->buf[indexTwo] = temp;
}

void sortVector(IntVector* vec)
{
    /*
      TODO: Implement Insertion Sort to sort the vector. The swap function will
      be useful here.
     */
    int i = 1;
    while (i < vec->len) {
        int j = i;
        while (j > 0 && vec->buf[j - 1] > vec->buf[j]) {
            swap(vec, j, j - 1);
            j--;
        }
        i++;
    }
}
void printVector(IntVector* vec)
{
    printf("(");
    for (size_t i = 0; i < vec->len; i++) {
        printf("%d ", vec->buf[i]);
    }
    printf(")\n");
}

int lengthVector(IntVector* vec)
{
    return vec->len;
}
