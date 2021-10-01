#include "hamming.h"
#include <assert.h>
#include <stdio.h>

/* Q1: read a string of arbitrary length from the standard input. The string is
   linefeed (\n) terminated. Think carefully of where memory is coming from as
   there is no upper bound on the string length. input: none ouput: a pointer to
   a buffer container the string read. It ought to be a valid string
              (\0 terminated)
*/
char* readString()
{
    size_t lcap = 8;
    char* line = malloc(lcap);
    char* read_pos = line;

    while (1) {
        size_t space = lcap - (read_pos - line);
        fgets(read_pos, space, stdin);

        char* endl = strchr(line, '\n');
        if (endl) {
            *endl = '\0';
            return line;
        }

        endl = strchr(line, '\0');
        if (endl - line + 1 >= lcap) {
            lcap *= 2;
            line = realloc(line, lcap);
            read_pos = strchr(line, '\0');
        }
        else {
            assert(0); // unreachable. hopefully.
        }
    }
}

/* Q2: compute the hamming distance between two strings. The strings are
   expected to be the same length. The hamming distance is the number of
   indices where the string differ. input: s1 : pointer to string 1 s2 :
   pointer to string 2 output: integer: the number of indices where the
   string disagree Note: 1. this should work for strings of any length.
             2. You can assume that s1 and s2 have the same length.
*/

int computeHD(char* s1, char* s2)
{
    int dist = 0;
    while (*s1 && *s2) {
        if (*s1 != *s2) {
            dist++;
        }
        s1++;
        s2++;
    }
    if ((!*s1) ^ (!*s2)) {
        dist++;
    }

    return dist;
}
