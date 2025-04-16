#ifndef COMPLEX_NUMBER_H
#define COMPLEX_NUMBER_H

#include <iostream>
#include <cmath>
#include <stdexcept>

class complex_number final
{
private:

	double _real;

	double _imaginary;

public:

	complex_number operator+(complex_number const&) const;

	complex_number& operator+=(complex_number const&);

	complex_number operator-(complex_number const&) const;

	complex_number& operator-=(complex_number const&);

	complex_number operator*(complex_number const&) const;

	complex_number& operator*=(complex_number const&);

	complex_number operator/(complex_number const&) const;

	complex_number& operator/=(complex_number const&);

	double modulus() const;

	double argument() const;

	friend std::ostream& operator<<(
		std::ostream&, 
		complex_number const&);

	friend std::istream& operator>>(
		std::istream&,
		complex_number&);

	complex_number(
		double const = 0, 
		double const = 0);

	complex_number(complex_number const&);

	complex_number& operator=(complex_number const&);

	~complex_number() noexcept;
};

#endif // COMPLEX_NUMBER_H