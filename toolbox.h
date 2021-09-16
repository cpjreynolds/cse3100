#ifndef CR_TOOLBOX_H
#define CR_TOOLBOX_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int prompt(const char* pmpt, const char* ifmt, ...)
{
    va_list ap;
    va_start(ap, ifmt);
    printf("%s", pmpt);
    int rc = vscanf(ifmt, ap);
    va_end(ap);
    return rc;
}

#endif
