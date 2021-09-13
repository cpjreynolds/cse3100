#include <stdio.h>

// I genuinely couldn't think of a way to do it without recursion.
int powmod(int n, int x, int m) {
    if (n == 0) {
        return 0;
    } else if (n == 1) {
        return 1;
    }
    if (x == 0) {
        return 1;
    } else if (x == 1) {
        return n % m;
    }

    if (x % 2 == 0) { // even
        return powmod((n * n) % m, x / 2, m) % m;
    } else { // odd
        return n * (powmod((n * n) % m, x / 2, m) % m) % m;
    }
}

int main(int argc, char* argv[]) {
    printf("Please enter n, e and m: ");
    int n, e, m;
    if (scanf("%d %d %d", &n, &e, &m) == 3) {
        int r = powmod(n, e, m);
        printf("%d ** %d mod %d = %d\n", n, e, m, r);
    } else {
        printf("You did not enter three integers. Exiting\n");
    }
}
