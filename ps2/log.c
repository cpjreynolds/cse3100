#include <stdio.h>


double approxLog(double a, int n)
{
  /*
    TODO: Use trapezoid integration to approximate the logarithm.
   */
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
