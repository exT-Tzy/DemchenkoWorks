#include <stdio.h>
#include <math.h>

double recursive_dichotomy_method(
	double a,
	double b,
	double (*f)(double),
	double eps)
{
	double mid = (a + b) / 2.0;
	double f_mid = f(mid);

	if (fabs(b - a) < eps || fabs(f_mid) < eps) return mid;

	if (f(a) * f_mid < 0) return recursive_dichotomy_method(a, mid, f, eps);
	else return recursive_dichotomy_method(mid, b, f, eps);
}

double equation(double x)
{
	return x * x - 2;
}

int main()
{
	double a = 0, b = 2, eps = 1e-6;

	printf("%f", recursive_dichotomy_method(a, b, equation, eps));

	return 0;
}