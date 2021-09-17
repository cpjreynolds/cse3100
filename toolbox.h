#ifndef CR_TOOLBOX_H
#define CR_TOOLBOX_H

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
/*
 * varargs are first passed to printf for the prompt, then scanf to read back.
 */
int prompt(const char* pmpt, const char* ifmt, ...)
{
    va_list ap;
    va_start(ap, ifmt);
    int rc = vprintf(pmpt, ap);
    assert(rc >= 0); // if printf fails then there are bigger issues at hand.
    rc = vscanf(ifmt, ap);
    va_end(ap);
    return rc;
}

#endif
