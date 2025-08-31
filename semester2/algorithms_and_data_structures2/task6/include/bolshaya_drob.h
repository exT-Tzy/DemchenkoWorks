#ifndef BOLSHAYA_DROB_H
#define BOLSHAYA_DROB_H

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <boost/multiprecision/cpp_int.hpp>

class bolshaya_drob final
{
private:

	boost::multiprecision::int1024_t _numerator;

	boost::multiprecision::int1024_t _denominator;

private:

	static std::map<bolshaya_drob, bolshaya_drob> _pi_cache;

	static std::vector<bolshaya_drob> _Bernoulli_cache;

private:

	void simplify();

private:

	static boost::multiprecision::int1024_t gcd(
		boost::multiprecision::int1024_t a,
		boost::multiprecision::int1024_t b);

public:

	static const bolshaya_drob DEFAULT_EPS;

public:

	bolshaya_drob& operator+=(
		bolshaya_drob const& other)&;

	bolshaya_drob& operator-=(
		bolshaya_drob const& other)&;

	bolshaya_drob& operator*=(
		bolshaya_drob const& other)&;

	bolshaya_drob& operator/=(
		bolshaya_drob const& other)&;


	bolshaya_drob operator+(
		bolshaya_drob const& other);

	bolshaya_drob operator-(
		bolshaya_drob const& other);

	bolshaya_drob operator*(
		bolshaya_drob const& other);

	bolshaya_drob operator/(
		bolshaya_drob const& other);


	bolshaya_drob operator-() const;

	bolshaya_drob& negate();


	bolshaya_drob abs() const;

	bolshaya_drob reciprocal() const;

	bolshaya_drob truncate() const;

public: 

	friend bool operator==(
		bolshaya_drob const& first,
		bolshaya_drob const& second);

	friend bool operator!=(
		bolshaya_drob const& first,
		bolshaya_drob const& second);

	friend bool operator<(
		bolshaya_drob const& first,
		bolshaya_drob const& second);

	friend bool operator<=(
		bolshaya_drob const& first,
		bolshaya_drob const& second);

	friend bool operator>(
		bolshaya_drob const& first,
		bolshaya_drob const& second);

	friend bool operator>=(
		bolshaya_drob const& first,
		bolshaya_drob const& second);

public:

	friend std::ostream& operator<<(
		std::ostream& out,
		bolshaya_drob const& number) noexcept;

	friend std::istream& operator>>(
		std::istream& in,
		bolshaya_drob& number);

	std::string to_decimal(
		size_t precision = 10) const;

public:

	friend bolshaya_drob pow(
		bolshaya_drob const& base,
		boost::multiprecision::int1024_t const& exp);

	friend bolshaya_drob sqrt(
		bolshaya_drob const& radicand,
		bolshaya_drob const& EPS);

public:

	bolshaya_drob();

	bolshaya_drob(
		boost::multiprecision::int1024_t const& numerator,
		boost::multiprecision::int1024_t const& denominator);

	bolshaya_drob(
		boost::multiprecision::int1024_t const& other);

	bolshaya_drob(
		int other);

	bolshaya_drob(
		bolshaya_drob const& other) = default;

	bolshaya_drob(
		bolshaya_drob&& other) = default;

	~bolshaya_drob() noexcept = default;


	bolshaya_drob& operator=(
		bolshaya_drob const& other) = default;

	bolshaya_drob& operator=(
		bolshaya_drob&& other) = default;
};

#endif //BOLSHAYA_DROB_H