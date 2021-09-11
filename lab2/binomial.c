#include <stdio.h>

int binomial(int n, int k){
    if (k == 0 || n == k)
        return 1;

    return binomial(n - 1, k) + binomial(n - 1, k - 1);
}

int main(int argc, char* argv[]){
    int n, k;
    printf("Enter n: ");
    scanf("%d", &n);
    printf("Enter k: ");
    scanf("%d", &k);
    printf("%d choose %d = %d\n", n, k, binomial(n, k));
}
