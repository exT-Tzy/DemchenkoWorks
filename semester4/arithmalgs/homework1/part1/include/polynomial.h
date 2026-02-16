#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

class polynomial
{
private:
    std::vector<double> coeffs;

    void remove_leading_zeros()
    {
        while (coeffs.size() > 1 && std::abs(coeffs.back()) < 1e-10)
        {
            coeffs.pop_back();
        }

        if (coeffs.empty())
        {
            coeffs.push_back(0);
        }
    }

public:
    polynomial() : coeffs(1, 0.0) {}

    polynomial(const std::vector<double> &c)
        : coeffs(c)
    {
        if (coeffs.empty())
        {
            coeffs.push_back(0);
        }

        remove_leading_zeros();
    }

    polynomial(double c) : coeffs(1, c) {}

    int degree() const
    {
        return coeffs.size() - 1;
    }

    double operator()(double x) const
    {
        double result = 0.0;
        double power = 1.0;

        for (size_t i = 0; i < coeffs.size(); i++)
        {
            result += coeffs[i] * power;
            power *= x;
        }

        return result;
    }

    double get_coeff(int i) const
    {
        if (i < 0 || i >= (int)coeffs.size())
        {
            return 0.0;
        }

        return coeffs[i];
    }

    void set_coeff(int i, double val)
    {
        if (i < 0)
        {
            return;
        }

        if (i >= (int)coeffs.size())
        {
            coeffs.resize(i + 1, 0.0);
        }

        coeffs[i] = val;
        remove_leading_zeros();
    }

    polynomial operator+(const polynomial &other) const
    {
        std::vector<double> result(std::max(coeffs.size(), other.coeffs.size()), 0.0);

        for (size_t i = 0; i < coeffs.size(); i++)
        {
            result[i] += coeffs[i];
        }

        for (size_t i = 0; i < other.coeffs.size(); i++)
        {
            result[i] += other.coeffs[i];
        }

        return polynomial(result);
    }

    polynomial operator-(const polynomial &other) const
    {
        std::vector<double> result(std::max(coeffs.size(), other.coeffs.size()), 0.0);

        for (size_t i = 0; i < coeffs.size(); i++)
        {
            result[i] += coeffs[i];
        }

        for (size_t i = 0; i < other.coeffs.size(); i++)
        {
            result[i] -= other.coeffs[i];
        }

        return polynomial(result);
    }

    polynomial operator*(double scalar) const
    {
        std::vector<double> result = coeffs;

        for (auto &c : result)
        {
            c *= scalar;
        }

        return polynomial(result);
    }

    polynomial operator*(const polynomial &other) const
    {
        if (degree() < 0 || other.degree() < 0)
        {
            return polynomial(0.0);
        }

        std::vector<double> result(coeffs.size() + other.coeffs.size() - 1, 0.0);

        for (size_t i = 0; i < coeffs.size(); i++)
        {
            for (size_t j = 0; j < other.coeffs.size(); j++)
            {
                result[i + j] += coeffs[i] * other.coeffs[j];
            }
        }

        return polynomial(result);
    }

    polynomial shift(double a) const
    {
        polynomial result(0.0);
        polynomial x_minus_a({-a, 1.0});
        polynomial power(1.0);

        for (size_t i = 0; i < coeffs.size(); i++)
        {
            result = result + power * coeffs[i];
            power = power * x_minus_a;
        }

        return result;
    }

    polynomial derivative() const
    {
        if (degree() == 0)
        {
            return polynomial(0.0);
        }

        std::vector<double> result(coeffs.size() - 1);

        for (size_t i = 1; i < coeffs.size(); i++)
        {
            result[i - 1] = coeffs[i] * i;
        }

        return polynomial(result);
    }
};

polynomial operator*(double scalar, const polynomial &p)
{
    return p * scalar;
}

#endif