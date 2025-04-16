#include "complex_number.h"

complex_number complex_number::operator+(complex_number const& other) const
{
	return complex_number(_real + other._real, _imaginary + other._imaginary);
}

complex_number& complex_number::operator+=(complex_number const& other)
{
	_real += other._real;
	_imaginary += other._imaginary;

	return *this;
}

complex_number complex_number::operator-(complex_number const& other) const
{
	return complex_number(_real - other._real, _imaginary - other._imaginary);
}

complex_number& complex_number::operator-=(complex_number const& other)
{
	_real -= other._real;
	_imaginary -= other._imaginary;

	return *this;
}

complex_number complex_number::operator*(complex_number const& other) const
{
	return complex_number(
		_real * other._real - _imaginary * other._imaginary, 
		_real * other._imaginary + _imaginary * other._real);
}

complex_number& complex_number::operator*=(complex_number const& other)
{
	double newr = _real * other._real - _imaginary * other._imaginary;
	double newi = _real * other._imaginary + _imaginary * other._real;

	_real = newr;
	_imaginary = newi;

	return *this;
}

complex_number complex_number::operator/(complex_number const& other) const
{
	double denominator = other._real * other._real + other._imaginary * other._imaginary;

	return complex_number(
		(_real * other._real + _imaginary * other._imaginary) / denominator,
		(_imaginary * other._real - _real * other._imaginary) / denominator);
}

complex_number& complex_number::operator/=(complex_number const& other)
{
	double denominator = other._real * other._real + other._imaginary * other._imaginary;
	double newr = (_real * other._real + _imaginary * other._imaginary) / denominator;
	double newi = (_imaginary * other._real - _real * other._imaginary) / denominator;

	_real = newr;
	_imaginary = newi;

	return *this;
}

double complex_number::modulus() const
{
	return sqrt(_real * _real + _imaginary * _imaginary);
}

double complex_number::argument() const
{
	if (_real == 0.0 || _imaginary == 0.0) throw std::runtime_error("An argument not defined!");

	return atan2(_imaginary, _real);
}

std::ostream& operator<<(
	std::ostream& os,
	complex_number const& src)
{
	os << src._real << '+' << src._imaginary << 'i';

	return os;
}

std::istream& operator>>(
	std::istream& is,
	complex_number& dest)
{
	std::cout << "Enter a real part of a complex number: ";
	is >> dest._real;
	std::cout << "Enter an imaginary part of a complex number: ";
	is >> dest._imaginary;

	return is;
}

complex_number::complex_number(
	double const real,
	double const imag)
{
	_real = real;
	_imaginary = imag;
}

complex_number::complex_number(complex_number const& other) = default;

complex_number& complex_number::operator=(complex_number const& other) = default;

complex_number::~complex_number() noexcept = default;