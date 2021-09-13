#include <math.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    double x, min = INFINITY, max = -INFINITY;
    int i = 0;
    while (scanf("%lf", &x) == 1) {
        /* TODO: Compute the min and max, and printf them */

        if (x > max)
            max = x;

        if (x < min)
            min = x;

        printf("Min=%lf Max=%lf\n", min, max);
    }
}
