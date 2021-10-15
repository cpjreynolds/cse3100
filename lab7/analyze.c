#include "analyze.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void analyzeURL(char* url, char* host, int* port, char* doc)
{
    /* TODO: Implement the URL decomposition.
     * input: URL the given url
       output: host : where you place the host part of the URL
               port : where you place the port number (if any)
               doc  : where you place the document name
    */
    if (strstr(url, "http://")) {
        url = url + sizeof("http://") - 1;
    }
    if (sscanf(url, "%512[^:]:%i%512s", host, port, doc) !=
        3) { // hilariously unsafe
        assert(sscanf(url, "%512[^/]%512s", host, doc) == 2); // lol
    }
}
