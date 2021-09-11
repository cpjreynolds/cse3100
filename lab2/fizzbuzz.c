#include <stdio.h>

#define START 1
#define END 100

int main(void) {
    for (int i = START; i <= END; i++) {
        if (i % 3 == 0) {
            printf("Fizz");
        }
        if (i % 5 == 0) {
            printf("Buzz");
        }
        if (i % 5 && i % 3) {
            printf("%d", i);
        }
        printf("\n");
    }
    return 0;
}
