#include "toolbox.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    int a = 2, b, c = 0;
    int rc = prompt("Enter %d integers: ", "%d %d", a, &b, &c);
    printf("%d integers: %d %d\n", a, b, c);
}
