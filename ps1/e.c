#include <math.h>
#include <stdio.h>

static inline unsigned long factorial(unsigned long n) {
    unsigned long ret = 1;
    for (; n > 1; n--) {
        ret *= n;
    }
    return ret;
}

int main(int argc, char* argv[]) {
    printf("n = ");
    int n;
    scanf("%u", &n);

    double e = 0;
    for (unsigned long k = 0; k <= n; k++) {
        e += 1.0 / factorial(k);
    }
    printf("e =  %lf\n", e);
}
