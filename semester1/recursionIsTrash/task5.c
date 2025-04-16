#include <stdio.h>

double binomial_coefficient_1(
    int n, 
    int k) 
{
    if (k > n || k < 0) return 0;
    if (k == 0) return 1;

    return ((double)(n - k + 1) / k) * binomial_coefficient_1(n, k - 1);
}

double binomial_coefficient_2(
    int n, 
    int k) 
{
    if (k > n || k < 0) return 0;
    if (k == 0) return 1;

    return ((double)(n - k + 1) / k) * binomial_coefficient_2(n, k - 1);
}

double binomial_coefficient_3(
    int n, 
    int k) 
{
    if (k > n || k < 0) return 0;
    if (k == 0 || k == n) return 1;

    return binomial_coefficient_3(n - 1, k - 1) + binomial_coefficient_3(n - 1, k);
}

int main()
{
    int n = 10, k = 2;

    printf("%lf %lf %lf", binomial_coefficient_1(n, k), binomial_coefficient_2(n, k), binomial_coefficient_3(n, k));

    return 0;
}