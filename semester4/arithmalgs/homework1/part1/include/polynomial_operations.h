#ifndef POLYNOMIAL_OPERATIONS_H
#define POLYNOMIAL_OPERATIONS_H

#include "polynomial.h"
#include <iostream>
#include <iomanip>
#include <limits>

class polynomial_operations
{
public:
    static bool check_linear_span(const polynomial &f, const std::vector<polynomial> &generators)
    {
        int max_degree = f.degree();
        for (const auto &g : generators)
        {
            max_degree = std::max(max_degree, g.degree());
        }

        int n = generators.size();
        int m = max_degree + 1;

        std::vector<std::vector<double>> matrix(m, std::vector<double>(n + 1, 0.0));

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                matrix[i][j] = generators[j].get_coeff(i);
            }
            matrix[i][n] = f.get_coeff(i);
        }

        for (int col = 0; col < std::min(m, n); col++)
        {
            int pivot_row = -1;
            for (int row = col; row < m; row++)
            {
                if (std::abs(matrix[row][col]) > 1e-10)
                {
                    pivot_row = row;
                    break;
                }
            }

            if (pivot_row == -1)
                continue;

            if (pivot_row != col)
            {
                std::swap(matrix[col], matrix[pivot_row]);
            }

            double pivot = matrix[col][col];
            for (int j = 0; j <= n; j++)
            {
                matrix[col][j] /= pivot;
            }

            for (int row = 0; row < m; row++)
            {
                if (row == col)
                    continue;
                double factor = matrix[row][col];
                for (int j = 0; j <= n; j++)
                {
                    matrix[row][j] -= factor * matrix[col][j];
                }
            }
        }

        for (int row = 0; row < m; row++)
        {
            bool all_zero = true;
            for (int col = 0; col < n; col++)
            {
                if (std::abs(matrix[row][col]) > 1e-10)
                {
                    all_zero = false;
                    break;
                }
            }
            if (all_zero && std::abs(matrix[row][n]) > 1e-10)
            {
                return false;
            }
        }

        return true;
    }

    static std::vector<double> find_linear_combination(const polynomial &f, const std::vector<polynomial> &generators)
    {
        int n = generators.size();
        int max_degree = f.degree();
        for (const auto &g : generators)
        {
            max_degree = std::max(max_degree, g.degree());
        }
        int m = max_degree + 1;

        std::vector<std::vector<double>> matrix(m, std::vector<double>(n + 1, 0.0));

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                matrix[i][j] = generators[j].get_coeff(i);
            }
            matrix[i][n] = f.get_coeff(i);
        }

        std::vector<int> pivot_cols(m, -1);

        for (int col = 0; col < n; col++)
        {
            int pivot_row = -1;
            for (int row = 0; row < m; row++)
            {
                bool row_used = false;
                for (int c = 0; c < col; c++)
                {
                    if (pivot_cols[row] >= 0)
                    {
                        row_used = true;
                        break;
                    }
                }
                if (!row_used && std::abs(matrix[row][col]) > 1e-10)
                {
                    pivot_row = row;
                    break;
                }
            }

            if (pivot_row == -1)
                continue;

            pivot_cols[pivot_row] = col;

            double pivot = matrix[pivot_row][col];
            for (int j = 0; j <= n; j++)
            {
                matrix[pivot_row][j] /= pivot;
            }

            for (int row = 0; row < m; row++)
            {
                if (row == pivot_row)
                    continue;
                double factor = matrix[row][col];
                for (int j = 0; j <= n; j++)
                {
                    matrix[row][j] -= factor * matrix[pivot_row][j];
                }
            }
        }

        std::vector<double> coefficients(n, 0.0);
        for (int row = 0; row < m; row++)
        {
            if (pivot_cols[row] >= 0)
            {
                coefficients[pivot_cols[row]] = matrix[row][n];
            }
        }

        return coefficients;
    }

    static polynomial represent_as_powers(const polynomial &f, double a)
    {
        std::vector<double> result_coeffs;
        polynomial temp = f;
        double factorial = 1.0;

        for (int i = 0; i <= f.degree(); i++)
        {
            if (i > 0)
            {
                factorial *= i;
            }

            double coeff = temp(a) / factorial;
            result_coeffs.push_back(coeff);

            if (i < f.degree())
            {
                temp = temp.derivative();
            }
        }

        return polynomial(result_coeffs);
    }

    static polynomial change_base(const polynomial &f, double old_base, double new_base)
    {
        polynomial in_powers = represent_as_powers(f, old_base);

        polynomial result(0.0);
        polynomial x_minus_b({-new_base, 1.0});
        polynomial power(1.0);

        for (int i = 0; i <= in_powers.degree(); i++)
        {
            result = result + power * in_powers.get_coeff(i);
            power = power * x_minus_b;
        }

        return result;
    }

    static double limit_at_point(const polynomial &numerator, const polynomial &denominator, double a)
    {
        polynomial num = numerator;
        polynomial den = denominator;

        while (std::abs(den(a)) < 1e-10 && std::abs(num(a)) < 1e-10)
        {
            num = num.derivative();
            den = den.derivative();

            if (den.degree() < 0 || (den.degree() == 0 && std::abs(den.get_coeff(0)) < 1e-10))
            {
                return std::numeric_limits<double>::quiet_NaN();
            }
        }

        if (std::abs(den(a)) < 1e-10)
        {
            return std::numeric_limits<double>::infinity();
        }

        return num(a) / den(a);
    }

    static double limit_at_infinity(const polynomial &numerator, const polynomial &denominator)
    {
        int num_deg = numerator.degree();
        int den_deg = denominator.degree();

        if (num_deg < den_deg)
        {
            return 0.0;
        }
        else if (num_deg == den_deg)
        {
            return numerator.get_coeff(num_deg) / denominator.get_coeff(den_deg);
        }
        else
        {
            double sign = (numerator.get_coeff(num_deg) * denominator.get_coeff(den_deg) > 0) ? 1.0 : -1.0;
            return sign * std::numeric_limits<double>::infinity();
        }
    }

    static double composite_limit_at_point(const polynomial &f1, const polynomial &s1,
                                           const polynomial &f2, const polynomial &s2,
                                           double a, int k, int l)
    {
        double s1_at_a = s1(a);
        double s2_at_a = s2(a);

        polynomial num_inner = f1;
        polynomial den_inner = s1;
        for (int i = 0; i < k; i++)
        {
            if (std::abs(den_inner(s1_at_a)) < 1e-10 && std::abs(num_inner(s1_at_a)) < 1e-10)
            {
                num_inner = num_inner.derivative();
                den_inner = den_inner.derivative();
            }
        }

        polynomial num_outer = f2;
        polynomial den_outer = s2;
        for (int i = 0; i < l; i++)
        {
            if (std::abs(den_outer(s2_at_a)) < 1e-10 && std::abs(num_outer(s2_at_a)) < 1e-10)
            {
                num_outer = num_outer.derivative();
                den_outer = den_outer.derivative();
            }
        }

        if (std::abs(den_outer(s2_at_a)) < 1e-10)
        {
            return std::numeric_limits<double>::infinity();
        }

        if (std::abs(den_inner(s1_at_a)) < 1e-10)
        {
            return std::numeric_limits<double>::infinity();
        }

        double ratio1 = num_inner(s1_at_a) / den_inner(s1_at_a);
        double ratio2 = num_outer(s2_at_a) / den_outer(s2_at_a);

        return ratio1 / ratio2;
    }

    static double composite_limit_at_infinity(const polynomial &f1, const polynomial &s1,
                                              const polynomial &f2, const polynomial &s2,
                                              int k, int l)
    {
        int f1_deg = f1.degree();
        int s1_deg = s1.degree();
        int f2_deg = f2.degree();
        int s2_deg = s2.degree();

        int num_power = k * f1_deg * s1_deg;
        int den_power = l * f2_deg * s2_deg;

        if (num_power < den_power)
        {
            return 0.0;
        }
        else if (num_power == den_power)
        {
            double num_lead = std::pow(f1.get_coeff(f1_deg) * std::pow(s1.get_coeff(s1_deg), f1_deg), k);
            double den_lead = std::pow(f2.get_coeff(f2_deg) * std::pow(s2.get_coeff(s2_deg), f2_deg), l);
            return num_lead / den_lead;
        }
        else
        {
            return std::numeric_limits<double>::infinity();
        }
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
};

#endif