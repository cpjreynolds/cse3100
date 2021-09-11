#include <stdio.h>
#include <stdlib.h>

#define N 10000
#define randPoint(a, b) (a + (b - a)*(rand() % RAND_MAX)/RAND_MAX)
#define randX(k) (randPoint(1.0, k))
#define randY (randPoint(0.0, 1.0))
#define sample(k) (randY <= 1.0/randX(k))

int main(){
    srand(50);
    printf("Enter number to take ln of: ");
    double k;
    scanf("%lf", &k);
    unsigned int m = 0;

    for(unsigned int i = 0; i < N; i++){
        if(sample(k)){
            m++;
        }

    }
    printf("Approximate logarithm: %lf\n", (k - 1.0)*m/N);
}


