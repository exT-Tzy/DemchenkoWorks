#include "bolshaya_drob.h"

#include <future>
#include <numeric>
#include <cstring>

const bolshaya_drob bolshaya_drob::DEFAULT_EPS = bolshaya_drob(boost::multiprecision::int1024_t(1), boost::multiprecision::int1024_t(1000000));

std::map<bolshaya_drob, bolshaya_drob> bolshaya_drob::_pi_cache;
std::vector<bolshaya_drob> bolshaya_drob::_Bernoulli_cache = { bolshaya_drob(1), bolshaya_drob(-1, 2) };

bolshaya_drob::bolshaya_drob()
{
    _numerator = 0;
    _denominator = 1;
}

bolshaya_drob::bolshaya_drob(
    int other) : bolshaya_drob(boost::multiprecision::int1024_t(other)) {}

bolshaya_drob::bolshaya_drob(
    const boost::multiprecision::int1024_t& numerator,
    const boost::multiprecision::int1024_t& denominator)
{
    if (denominator == 0)
    {
        throw std::invalid_argument("Denominator cannot be 0");
    }

    _numerator = numerator;
    _denominator = denominator;

    if (_denominator < 0) 
    {
        _numerator = -_numerator;
        _denominator = -_denominator;
    }

    simplify();
}

bolshaya_drob::bolshaya_drob(
    const boost::multiprecision::int1024_t& other)
    : _numerator(other), _denominator(1)
{}

void bolshaya_drob::simplify()
{
    if (_numerator == 0)
    {
        _denominator = 1;
        
        return;
    }

    boost::multiprecision::int1024_t divider = gcd(_numerator, _denominator);

    if (divider > 1)
    {
        _numerator /= divider;
        _denominator /= divider;
    }
}

bolshaya_drob bolshaya_drob::operator-() const
{
    return bolshaya_drob(-_numerator, _denominator);
}

bolshaya_drob& bolshaya_drob::negate()
{
    _numerator = -_numerator;
   
    return *this;
}

bolshaya_drob& bolshaya_drob::operator+=(
    const bolshaya_drob& other)&
{
    _numerator = _numerator * other._denominator + other._numerator * _denominator;
    _denominator *= other._denominator;
    simplify();
   
    return *this;
}

bolshaya_drob bolshaya_drob::operator+(
    const bolshaya_drob& other)
{
    bolshaya_drob temp = *this;
   
    return temp += other;
}

bolshaya_drob& bolshaya_drob::operator-=(
    const bolshaya_drob& other)&
{
    return *this += -other;
}

bolshaya_drob bolshaya_drob::operator-(
    const bolshaya_drob& other)
{
    return *this + -other;
}

bolshaya_drob& bolshaya_drob::operator*=(
    const bolshaya_drob& other)&
{
    _numerator *= other._numerator;
    _denominator *= other._denominator;
    simplify();
   
    return *this;
}

bolshaya_drob bolshaya_drob::operator*(
    const bolshaya_drob& other)
{
    bolshaya_drob temp = *this;
    
    return temp *= other;
}

bolshaya_drob& bolshaya_drob::operator/=(
    const bolshaya_drob& other)&
{
    if (other._numerator == 0)
    {
        throw std::invalid_argument("Division by zero");
    }
   
    _numerator *= other._denominator;
    _denominator *= other._numerator;

    if (_denominator < 0) 
    {
        _numerator = -_numerator;
        _denominator = -_denominator;
    }

    simplify();
   
    return *this;
}

bolshaya_drob bolshaya_drob::operator/(
    const bolshaya_drob& other)
{
    bolshaya_drob temp = *this;
   
    return temp /= other;
}

bolshaya_drob bolshaya_drob::abs() const
{
    return _numerator < 0 ? -*this : *this;
}

bolshaya_drob bolshaya_drob::reciprocal() const
{
    if (_numerator == 0) 
    {
        throw std::invalid_argument("Cannot take reciprocal of zero");
    }
   
    return bolshaya_drob(_denominator, _numerator);
}

bolshaya_drob bolshaya_drob::truncate() const
{
    return bolshaya_drob(_numerator / _denominator, 1);
}

boost::multiprecision::int1024_t bolshaya_drob::gcd(
    boost::multiprecision::int1024_t a,
    boost::multiprecision::int1024_t b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;

    while (b != 0)
    {
        boost::multiprecision::int1024_t temp = b;
        b = a % b;
        a = temp;
    }
  
    return a;
}

std::string bolshaya_drob::to_decimal(size_t precision) const
{
    boost::multiprecision::int1024_t abs_num = _numerator < 0 ? -_numerator : _numerator;
    boost::multiprecision::int1024_t abs_den = _denominator;

    boost::multiprecision::int1024_t integer_part = abs_num / abs_den;
    boost::multiprecision::int1024_t remainder = abs_num % abs_den;

    std::string result;
   
    if (_numerator < 0)
    {
        result += "-";
    }

    result += integer_part.str();

    if (remainder != 0 && precision > 0)
    {
        result += ".";
       
        for (size_t i = 0; i < precision; ++i)
        {
            remainder *= 10;
            boost::multiprecision::int1024_t digit = remainder / abs_den;
            remainder %= abs_den;
            result += digit.str();
           
            if (remainder == 0) break;
        }
    }

    return result;
}

bool operator==(
    const bolshaya_drob& first, 
    const bolshaya_drob& second)
{
    return first._numerator * second._denominator == second._numerator * first._denominator;
}

bool operator!=(
    const bolshaya_drob& first, 
    const bolshaya_drob& second)
{
    return !(first == second);
}

bool operator<(
    const bolshaya_drob& first,
    const bolshaya_drob& second)
{
    return first._numerator * second._denominator < second._numerator * first._denominator;
}

bool operator<=(
    const bolshaya_drob& first, 
    const bolshaya_drob& second)
{
    return !(first > second);
}

bool operator>(
    const bolshaya_drob& first, 
    const bolshaya_drob& second)
{
    return first._numerator * second._denominator > second._numerator * first._denominator;
}

bool operator>=(
    const bolshaya_drob& first,
    const bolshaya_drob& second)
{
    return !(first < second);
}

bolshaya_drob pow(
    const bolshaya_drob& base,
    const boost::multiprecision::int1024_t& exp)
{
    if (exp == 0) return bolshaya_drob(1);
    if (exp < 0) return bolshaya_drob(1) / pow(base, -exp);

    bolshaya_drob result(1);
    bolshaya_drob x = base;
    boost::multiprecision::int1024_t n = exp;

    while (n > 0)
    {
        if (n % 2 == 1) result *= x;
        x *= x;
        n = n / 2;
    }
   
    return result;
}

bolshaya_drob sqrt(
    const bolshaya_drob& radicand, 
    const bolshaya_drob& EPS)
{
    if (radicand < bolshaya_drob(0)) 
    {
        throw std::invalid_argument("Square root of negative number");
    }
    if (radicand == bolshaya_drob(0))
    {
        return bolshaya_drob(0);
    }

    bolshaya_drob x = radicand;
    bolshaya_drob prev;

    do 
    {
        prev = x;
        x = (x + radicand / x) / bolshaya_drob(2);
    } 
    while ((x - prev).abs() > EPS);

    return x;
}

std::ostream& operator<<(std::ostream& out, const bolshaya_drob& num) noexcept
{
    if (num._denominator == 1) 
    {
        out << num._numerator;
    }
    else
    {
        out << num._numerator << "/" << num._denominator;
    }
    
    return out;
}

std::istream& operator>>(
    std::istream& in,
    bolshaya_drob& num)
{
    std::string input;
    in >> input;

    size_t slash_pos = input.find('/');

    if (slash_pos == std::string::npos) 
    {
        try 
        {
            boost::multiprecision::int1024_t numerator(input);
            num = bolshaya_drob(numerator);
        }
        catch (const std::exception&) 
        {
            in.setstate(std::ios::failbit);
        }
    }
    else 
    {
        try 
        {
            std::string num_str = input.substr(0, slash_pos);
            std::string den_str = input.substr(slash_pos + 1);

            boost::multiprecision::int1024_t numerator(num_str);
            boost::multiprecision::int1024_t denominator(den_str);

            num = bolshaya_drob(numerator, denominator);
        }
        catch (const std::exception&) 
        {
            in.setstate(std::ios::failbit);
        }
    }

    return in;
}