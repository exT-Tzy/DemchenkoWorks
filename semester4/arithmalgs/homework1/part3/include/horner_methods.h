#ifndef HORNER_METHODS_H
#define HORNER_METHODS_H

#include "polynomial.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>

class horner_methods
{
public:
    struct horner_result
    {
        polynomial quotient;
        double remainder;
        int additions;
        int multiplications;
    };

    static horner_result evaluate_with_division(const polynomial &p, double x0)
    {
        horner_result result;
        result.additions = 0;
        result.multiplications = 0;

        int n = p.degree();
        std::vector<double> b(n + 1);

        b[n] = p.get_coeff(n);

        for (int i = n - 1; i >= 0; i--)
        {
            b[i] = p.get_coeff(i) + x0 * b[i + 1];
            result.multiplications++;
            result.additions++;
        }

        result.remainder = b[0];

        std::vector<double> quotient_coeffs(b.begin() + 1, b.end());
        result.quotient = polynomial(quotient_coeffs);

        return result;
    }

    static std::pair<polynomial, double> divide_by_linear(const polynomial &p, double x0)
    {
        auto result = evaluate_with_division(p, x0);
        return {result.quotient, result.remainder};
    }

    static horner_result evaluate_shifted(const polynomial &p, double x, double x0)
    {
        horner_result result;
        result.additions = 0;
        result.multiplications = 0;

        int n = p.degree();
        std::vector<double> c(n + 1);

        c[n] = p.get_coeff(n);

        for (int i = n - 1; i >= 0; i--)
        {
            c[i] = p.get_coeff(i) + x0 * c[i + 1];
            result.multiplications++;
            result.additions++;
        }

        double h = x - x0;
        double value = c[0];

        for (int i = 1; i <= n; i++)
        {
            value = value + c[i] * h;
            result.multiplications++;
            result.additions++;
            if (i < n)
            {
                h = h * (x - x0);
                result.multiplications++;
            }
        }

        result.remainder = value;
        return result;
    }

    static std::vector<polynomial> compute_taylor_polynomials(const polynomial &p, double x0, int max_degree)
    {
        std::vector<polynomial> taylor_polys;

        polynomial current = p;
        double factorial = 1.0;

        for (int k = 0; k <= max_degree && current.degree() >= 0; k++)
        {
            if (k > 0)
            {
                factorial *= k;
            }

            double coeff = current(x0) / factorial;

            std::vector<double> taylor_coeffs(k + 1, 0.0);
            taylor_coeffs[k] = coeff;
            taylor_polys.push_back(polynomial(taylor_coeffs));

            if (current.degree() > 0)
            {
                current = current.derivative();
            }
            else
            {
                break;
            }
        }

        return taylor_polys;
    }

    static polynomial evaluate_bivariate_horner(const std::vector<std::vector<double>> &coeffs, double x, double y)
    {
        int n = coeffs.size() - 1;

        std::vector<double> result_coeffs;

        for (int i = n; i >= 0; i--)
        {
            double val = 0.0;
            int m = coeffs[i].size() - 1;

            for (int j = m; j >= 0; j--)
            {
                val = val * y + coeffs[i][j];
            }

            result_coeffs.insert(result_coeffs.begin(), val);
        }

        return polynomial(result_coeffs);
    }

    static std::pair<polynomial, polynomial> divide_by_quadratic(const polynomial &p, double alpha, double beta)
    {
        int n = p.degree();
        if (n < 2)
        {
            return {polynomial(0.0), p};
        }

        std::vector<double> q(n - 1);
        std::vector<double> r(2);

        q[n - 2] = p.get_coeff(n);

        if (n >= 3)
        {
            q[n - 3] = p.get_coeff(n - 1) + alpha * q[n - 2];
        }

        for (int i = n - 4; i >= 0; i--)
        {
            q[i] = p.get_coeff(i + 2) + alpha * q[i + 1] + beta * q[i + 2];
        }

        r[1] = p.get_coeff(1) + alpha * q[0];
        if (n >= 2)
        {
            r[1] += beta * q[1];
        }

        r[0] = p.get_coeff(0) + beta * q[0];

        return {polynomial(q), polynomial(r)};
    }

    static int count_operations_standard(int degree)
    {
        int mults = degree;
        int adds = degree;
        return mults + adds;
    }

    static int count_operations_horner(int degree)
    {
        return 2 * degree;
    }

    static void print_polynomial(const polynomial &p, const std::string &name = "")
    {
        if (!name.empty())
        {
            std::cout << name << " = ";
        }

        bool first = true;
        for (int i = p.degree(); i >= 0; i--)
        {
            double c = p.get_coeff(i);
            if (std::abs(c) < 1e-10)
                continue;

            if (!first && c > 0)
                std::cout << " + ";
            if (c < 0)
                std::cout << " - ";

            double abs_c = std::abs(c);
            if (std::abs(abs_c - 1.0) > 1e-10 || i == 0)
            {
                std::cout << abs_c;
            }

            if (i > 0)
            {
                std::cout << "x";
                if (i > 1)
                {
                    std::cout << "^" << i;
                }
            }

            first = false;
        }

        if (first)
        {
            std::cout << "0";
        }

        std::cout << std::endl;
    }

    static polynomial compute_factorial_polynomial(int k)
    {
        if (k == 0)
        {
            return polynomial(1.0);
        }

        polynomial result({0.0, 1.0});

        for (int i = 1; i < k; i++)
        {
            polynomial factor({-(double)i, 1.0});
            result = result * factor;
        }

        return result;
    }

    static horner_result evaluate_factorial_polynomial_form(const std::vector<double> &u_coeffs, double x)
    {
        horner_result result;
        result.additions = 0;
        result.multiplications = 0;

        int n = u_coeffs.size() - 1;

        double value = 0.0;
        double factorial_term = 1.0;

        for (int i = 0; i <= n; i++)
        {
            if (i > 0)
            {
                factorial_term *= (x - (i - 1));
                result.multiplications++;
            }

            value += u_coeffs[i] * factorial_term;
            result.multiplications++;
            result.additions++;
        }

        result.remainder = value;
        return result;
    }
};

#endif