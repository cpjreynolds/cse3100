#include <stdio.h>

double approxLog(double a, int n)
{
    /*
      TODO: Use trapezoid integration to approximate the logarithm.
     */
    double d_x = (a - 1.) / n;

    double sum = 0.0;
    for (int k = 1; k < n; k++) {
        double x_k = 1.0 + k * d_x;
        sum += 1.0 / x_k;
    }
    double x_n_recip = 1.0 / (1.0 + n * d_x);
    double x_0_recip = 1.0;
    double area = (x_n_recip + x_0_recip) / 2;
    return d_x * (area + sum);
}

int main(int argc, char* argv[])
{
    double a;
    int n;
    printf("Enter a: ");
    scanf("%lf", &a);
    printf("Enter n: ");
    scanf("%d", &n);
    double logarithm = approxLog(a, n);
    printf("Natural logarithm: %lf\n", logarithm);
    return 0;
}
