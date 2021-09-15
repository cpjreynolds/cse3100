#include <stdio.h>
/*
#define OCTANOL 4
#define BUTANOL 3
#define ETHANOL 2
#define METHANOL 1

int value(int coin) {
    if (coin == OCTANOL) {
        return 8;
    } else if (coin == BUTANOL) {
        return 4;
    } else if (coin == ETHANOL) {
        return 2;
    } else {
        return 1;
    }
}
*/

enum {
    METHANOL = 1,
    ETHANOL = 2,
    BUTANOL = 4,
    OCTANOL = 8,
};

int count(int x, int maxCoin) {
    /*
      TODO: Fill this in to count the number of ways to make change for amount
      cents using coins with value <= the value of maxCoin
     */
    if (x == 0) {
        return 1;
    } else if (x < 0) {
        return 0;
    }

    if (maxCoin == 1) {
        return count(x - 1, 1);
    }

    return count(x - maxCoin, maxCoin) + count(x, maxCoin / 2);
}

int main(int argc, char* argv[]) {
    int amount;
    printf("Enter amount: ");
    scanf("%d", &amount);
    printf("Number of ways to make %d cents: %d\n", amount,
           count(amount, OCTANOL));
}
