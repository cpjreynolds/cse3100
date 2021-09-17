#include <assert.h>
#include <stdio.h>

double leibniz_tail_recurrent(int, double);

double leibnizRecurrent(int n)
{
    /* TODO: Fill in a recurrent version. That is, compute the term for n plus
     * leibnizRecurrent(n - 1) */
    /*
    if (n == 0) {
        return 1.0;
    }
    double sign = n % 2 ? -1.0 : 1.0;
    double denom = 2 * n + 1;
    return sign / denom + leibnizRecurrent(n - 1);
    */
    return leibniz_tail_recurrent(n, 0.0);
}

// This is tail recursive when optimization is turned on and won't smash the
// stack with large `n` like the regular recursive version does.
double leibniz_tail_recurrent(int n, double sum)
{
    if (n == 0) {
        return sum + 1.0;
    }
    double sign = n % 2 ? -1.0 : 1.0;
    double denom = 2 * n + 1;
    sum += sign / denom;
    return leibniz_tail_recurrent(n - 1, sum);
}

double leibnizIterative(int n)
{
    /* TODO: Fill in the iterative version. That is, use a loop to compute the
     * summation. */

    double sum = 0.0;
    for (int k = 0, sign = 1; k <= n; k++, sign = -sign) {
        sum += sign / (2. * k + 1.);
    }

    return sum;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage: ./pi <mode> <n>, where <mode> is 1 for recurrent "
               "version, and 2 is for the iterative version, and n is the "
               "upper bound of the summation.\n");
    }
    else {
        int mode = 0, n = 0;
        double pi = 0;
        assert(sscanf(argv[1], "%d", &mode) == 1);
        assert(sscanf(argv[2], "%d", &n) == 1);

        assert(mode == 1 || mode == 2);
        assert(n >= 0);
        if (mode == 1) {
            pi = 4 * leibnizRecurrent(n);
        }
        else {
            pi = 4 * leibnizIterative(n);
        }
        printf("PI: %f\n", pi);
    }
    return 0;
}
