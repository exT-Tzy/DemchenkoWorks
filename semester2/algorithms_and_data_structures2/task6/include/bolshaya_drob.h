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

	static const bolshaya_drob& Bernoulli_number(
		size_t n);

	static bolshaya_drob binomial(
		size_t n,
		size_t k);

	static bolshaya_drob reduce_argument(
		bolshaya_drob const& number,
		bolshaya_drob const& EPS);

	static boost::multiprecision::int1024_t gcd(
		boost::multiprecision::int1024_t a,
		boost::multiprecision::int1024_t b);

public:

	static const bolshaya_drob DEFAULT_EPS;

	static void calculate_Bernoulli_number_up_to(
		size_t n);

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
	
	static bolshaya_drob PI(
		const bolshaya_drob& EPS = DEFAULT_EPS);

	friend bolshaya_drob sin(bolshaya_drob const& number, bolshaya_drob const& EPS);

	friend bolshaya_drob cos(bolshaya_drob const& number, bolshaya_drob const& EPS);

	friend bolshaya_drob tg(bolshaya_drob const& number, bolshaya_drob const& EPS);

	friend bolshaya_drob ctg(bolshaya_drob const& number, bolshaya_drob const& EPS);

	friend bolshaya_drob sec(bolshaya_drob const& number, bolshaya_drob const& EPS);

	friend bolshaya_drob cosec(bolshaya_drob const& number, bolshaya_drob const& EPS);

	friend bolshaya_drob arcsin(bolshaya_drob const& number, bolshaya_drob const& EPS);

	friend bolshaya_drob arccos(bolshaya_drob const& number, bolshaya_drob const& EPS);

	friend bolshaya_drob arctg(bolshaya_drob const& number, bolshaya_drob const& EPS);

	friend bolshaya_drob arcctg(bolshaya_drob const& number, bolshaya_drob const& EPS);

	friend bolshaya_drob arcsec(bolshaya_drob const& number, bolshaya_drob const& EPS);

	friend bolshaya_drob arccosec(bolshaya_drob const& number, bolshaya_drob const& EPS);

public:

	friend bolshaya_drob pow(
		bolshaya_drob const& base,
		boost::multiprecision::int1024_t const& exp);

	friend bolshaya_drob radical(
		bolshaya_drob const& radicand,
		boost::multiprecision::int1024_t const& index,
		bolshaya_drob const& EPS);

	friend bolshaya_drob sqrt(
		bolshaya_drob const& radicand,
		boost::multiprecision::int1024_t const& EPS);

public:

	friend bolshaya_drob log2(
		bolshaya_drob const& number,
		bolshaya_drob const& EPS);

	friend bolshaya_drob log(
		bolshaya_drob const& number,
		bolshaya_drob const& EPS);

	friend bolshaya_drob log10(
		bolshaya_drob const& bumber,
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