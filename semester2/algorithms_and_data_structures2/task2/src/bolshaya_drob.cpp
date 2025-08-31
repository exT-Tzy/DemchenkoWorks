#include "bolshaya_drob.h"

#include <future>
#include <numeric>

const bolshaya_drob bolshaya_drob::DEFAULT_EPS = bolshaya_drob(boost::multiprecision::int1024_t(1), boost::multiprecision::int1024_t(1000000));

std::map<bolshaya_drob, bolshaya_drob> bolshaya_drob::_pi_cache;

std::vector<bolshaya_drob> bolshaya_drob::_Bernoulli_cache = { 1, bolshaya_drob(-1, 2) };

bolshaya_drob::bolshaya_drob() 
{
    _numerator = 0;
    _denominator = 1;
}

bolshaya_drob::bolshaya_drob(
    int other) : 
    bolshaya_drob(boost::multiprecision::int1024_t(other)) 
{}

bolshaya_drob::bolshaya_drob(
    boost::multiprecision::int1024_t const& numerator, 
    boost::multiprecision::int1024_t const& denominator) 
{
    if (denominator == 0) 
    {
        throw std::invalid_argument("Demonimator can not be 0");
    }

    if (numerator >= 0 && denominator >= 0) 
    {
        _numerator = numerator;
        _denominator = denominator;
    }
    else if (numerator < 0 || denominator < 0) 
    {
        _numerator = -numerator;
        _denominator = -denominator;
    }
    else
    {
        _numerator = numerator < 0
            ? -numerator
            : numerator;
       
        _denominator = denominator < 0 
            ? denominator 
            : -denominator;
    }
    
    simplify();
}

bolshaya_drob::bolshaya_drob(
    boost::multiprecision::int1024_t const& other)
    : _numerator(other < 0 ? -other : other), _denominator(other < 0 ? -1 : 1)
{}

void bolshaya_drob::simplify() 
{
    if (_numerator == 0) 
    {
        _denominator = 1;
       
        return;
    }
    
    if (_numerator < 0) 
    {
        _numerator = -_numerator;
        _denominator = -_denominator;
    }
    
    boost::multiprecision::int1024_t divider = gcd(_numerator, _denominator < 0 ? -_denominator : _denominator);
   
    if (divider == 1) 
    {
        return;
    }
   
    _numerator /= divider;
    _denominator /= divider;
}
bolshaya_drob bolshaya_drob::operator-() const 
{
    bolshaya_drob negative = *this;
    
    return negative.negate();
}

bolshaya_drob& bolshaya_drob::negate() 
{
    _denominator = -_denominator;
   
    return *this;
}

bolshaya_drob& bolshaya_drob::operator+=(
    bolshaya_drob const& other)& 
{
    _numerator =
        _numerator * other._denominator + other._numerator * _denominator;
    _denominator *= other._denominator;

    simplify();

    return *this;
}

bolshaya_drob bolshaya_drob::operator+(
    bolshaya_drob const& other) 
{
    bolshaya_drob temp = *this;
    
    return temp += other;
}

bolshaya_drob& bolshaya_drob::operator-=(
    bolshaya_drob const& other)& 
{
    return *this += -other;
}

bolshaya_drob bolshaya_drob::operator-(
    bolshaya_drob const& other)
{
    return *this + -other;
}

bolshaya_drob& bolshaya_drob::operator*=(
    bolshaya_drob const& other)& 
{
    _numerator *= other._numerator;
    _denominator *= other._denominator;
    simplify();
    return *this;
}

bolshaya_drob bolshaya_drob::operator*(
    bolshaya_drob const& other)
{
    bolshaya_drob temp = *this;
    return temp *= other;
}

bolshaya_drob& bolshaya_drob::operator/=(
    bolshaya_drob const& other)& 
{
    if (other == bolshaya_drob(0)) 
    {
        throw std::invalid_argument("Division by zero is bad idea ._.");
    }
    _numerator *= other._denominator;
    _denominator *= other._numerator;
   
    simplify();
    
    return *this;
}

bolshaya_drob bolshaya_drob::operator/(
    bolshaya_drob const& other)
{
    bolshaya_drob temp = *this;
    
    return temp /= other;
}

bolshaya_drob bolshaya_drob::abs() const 
{ 
    return _denominator < 0 
        ? -*this 
        : *this; 
}

bolshaya_drob bolshaya_drob::reciprocal() const 
{
    bolshaya_drob res(_denominator, _numerator);
    
    if (res._numerator < 0) 
    {
        res._numerator = -res._numerator;
        res._denominator = -res._denominator;
    }
   
    return res;
}

bolshaya_drob bolshaya_drob::truncate() const 
{
    return { _numerator / _denominator, 1 }; 
}

boost::multiprecision::int1024_t bolshaya_drob::gcd(
    boost::multiprecision::int1024_t a,
    boost::multiprecision::int1024_t b)
{
    if (a == 0) 
    {
        return b < 0 
            ? -b 
            : b;
    }
   
    if (b == 0)
    {
        return a < 0 
            ? -a 
            : a;
    }

    if (a < 0) 
    {
        a = -a;
    }
   
    if (b < 0) 
    {
        b = -b;
    }

    while (b != 0) 
    {
        boost::multiprecision::int1024_t temp = b;
       
        b = a % b;
        a = temp;
    }

    return a;
}

bolshaya_drob pow(
    bolshaya_drob const& base, 
    boost::multiprecision::int1024_t const& exp) 
{
    if (exp == 0) 
    {
        return 1;
    }
    
    if (exp < 0) 
    {
        return bolshaya_drob(1) / pow(base, -exp);
    }
    
    bolshaya_drob result = 1;
    bolshaya_drob x = base;
   
    boost::multiprecision::int1024_t n = exp;

    while (n > 0) 
    {
        if (n % 2 == 1)
        {
            result *= x;
        }
       
        x *= x;
        n = n / 2;
    }

    return result;
}

bolshaya_drob radical(
    const bolshaya_drob& radicand, 
    const boost::multiprecision::int1024_t& index,
    const bolshaya_drob& EPS) 
{
    if (index == 0) 
    {
        throw std::invalid_argument("Index cannot be zero!");
    }
    
    if (const_cast<bolshaya_drob&>(radicand) < bolshaya_drob(0) && (index % 2 == 0)) 
    {
        throw std::invalid_argument("Negative radicand with even index");
    }

    bolshaya_drob approx = 1;
    
    while (pow(approx, index) < radicand.abs()) 
    {
        approx *= 2;
    }
    
    bolshaya_drob x = (const_cast<bolshaya_drob&>(radicand) < bolshaya_drob(0)) 
        ? -approx 
        : approx;

    bolshaya_drob delta;

    do 
    {
        bolshaya_drob x_prev = x;

        bolshaya_drob x_to_power = 1;
       
        for (boost::multiprecision::int1024_t i = 0; i < index - 1; ++i)
        {
            x_to_power *= x;
        }

        x = (bolshaya_drob(index - 1) * x + const_cast<bolshaya_drob&>(radicand) / x_to_power) / index;

        delta = (x - x_prev).abs();
    } 
    while (delta > EPS);

    return x;
}

bolshaya_drob sqrt(
    bolshaya_drob const& radicand,
    bolshaya_drob const& EPS) 
{
    return radical(radicand, 2, EPS);
}

std::ostream& operator<<(
    std::ostream& out,
    bolshaya_drob const& num) noexcept 
{
    if (num == bolshaya_drob(0)) 
    {
        out << 0;
        
        return out;
    }
   
    if (num._denominator < 0) 
    {
        out << "-";
    }
   
    if (num._denominator != 1) 
    {
        out << num._numerator << "/" << (num._denominator < 0) ? -num._denominator : num._denominator;
    }
    else 
    {
        out << num._numerator;
    }
   
    return out;
}
std::istream& operator>>(
    std::istream& in, 
    bolshaya_drob& num)
{
    std::string input;
    in >> input;

    char* slash_pos = std::strchr(input.data(), '/');
    
    if (slash_pos == nullptr) 
    {
        in.setstate(std::ios::failbit);
        
        return in;
    }
    *slash_pos = '\0';
    char* _numeratorstr = input.data();
    char* _denominatorstr = slash_pos + 1;

    char sign = '+';
    if (_numeratorstr[0] == '+' || _numeratorstr[0] == '-') 
    {
        sign = _numeratorstr[0];
        ++_numeratorstr;
    }
   
    try 
    {
        boost::multiprecision::int1024_t numerator(_numeratorstr, 10);
        boost::multiprecision::int1024_t denominator(_denominatorstr, 10);
        
        if (sign == '-') 
        {
            denominator = -denominator;
        }

        num._denominator = std::move(denominator);
        num._numerator = std::move(numerator);

    }
    catch (std::exception const& e) 
    {
        in.setstate(std::ios::failbit);
       
        return in;
    }
    
    return in;
}

std::string bolshaya_drob::to_decimal(
    size_t precision) const 
{
    bool is_negative = (_denominator < 0);

    boost::multiprecision::int1024_t abs_num = _numerator < 0 ? -_numerator : _numerator;
    boost::multiprecision::int1024_t abs_den = _denominator < 0 ? -_numerator : _numerator;

    boost::multiprecision::int1024_t integer_part = abs_num / abs_den;
    boost::multiprecision::int1024_t remainder = abs_num % abs_den;

    std::string result;

    if (is_negative) 
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

            if (remainder == 0) 
            {
                break;
            }
        }
    }

    return result;
}

bolshaya_drob log_taylor_series(
    const bolshaya_drob& x,
    const bolshaya_drob& EPS) 
{
    bolshaya_drob w = (const_cast<bolshaya_drob&>(x) - bolshaya_drob(1)) / (const_cast<bolshaya_drob&>(x) + bolshaya_drob(1));
    bolshaya_drob w_squared = w * w;
    bolshaya_drob term = w;
    bolshaya_drob result = w;
    bolshaya_drob n = bolshaya_drob(3);

    while (term.abs() > EPS) 
    {
        term *= w_squared;
        result += term / n;
        n += bolshaya_drob(2);
    }

    return result * bolshaya_drob(2);
}

bolshaya_drob log(bolshaya_drob const& number, bolshaya_drob const& EPS) {
    if (const_cast<bolshaya_drob&>(number) <= bolshaya_drob(0)) 
    {
        throw std::invalid_argument("logarithm of non-positive number");
    }

    bolshaya_drob x = number;
    boost::multiprecision::int1024_t k = 0;

    while (x > bolshaya_drob(1)) 
    {
        x /= bolshaya_drob(2);
        ++k;
    }
    
    while (x < bolshaya_drob(1, 2)) 
    {
        x *= bolshaya_drob(2);
        --k;
    }

    static bolshaya_drob ln2 = log_taylor_series(bolshaya_drob(2), EPS);
    
    return log_taylor_series(x, EPS) + bolshaya_drob(k) * ln2;
}

bolshaya_drob log2(
    bolshaya_drob const& number,
    bolshaya_drob const& EPS) 
{
    bolshaya_drob ln2 = log(2, EPS);
    
    return log(number, EPS) / ln2;
}

bolshaya_drob log10(
    bolshaya_drob const& number, 
    bolshaya_drob const& EPS) 
{
    if (const_cast<bolshaya_drob&>(number) <= bolshaya_drob(0)) 
    {
        throw std::invalid_argument("logarithm of non-positive number");
    }

    bolshaya_drob ln10 = log(bolshaya_drob(10), EPS);

    return log(number, EPS) / ln10;
}

bool operator==(
    bolshaya_drob const& first, 
    bolshaya_drob const& second) 
{
    return first._numerator == second._numerator &&
        first._denominator == second._denominator;
}

bool operator!=(
    bolshaya_drob const& first, 
    bolshaya_drob const& second) 
{
    return !(first == second);
}

bool operator<(
    bolshaya_drob const& first, 
    bolshaya_drob const& second) 
{
    if (first._denominator < 0 && second._denominator > 0) 
    {
        return true;
    }
    
    if (first._denominator > 0 && second._denominator < 0) 
    {
        return false;
    }
    
    boost::multiprecision::int1024_t new_first_numerator = first._numerator * second._denominator;
    boost::multiprecision::int1024_t new_second_numerator = second._numerator * first._denominator;
    
    return new_first_numerator < new_second_numerator;
}

bool operator<=(
    bolshaya_drob const& first, 
    bolshaya_drob const& second) 
{
    return !(first > second);
}

bool operator>(
    bolshaya_drob const& first, 
    bolshaya_drob const& second)
{
    if (first._denominator < 0 && second._denominator > 0) 
    {
        return false;
    }
   
    if (first._denominator > 0 && second._denominator < 0)
    {
        return true;
    }
   
    boost::multiprecision::int1024_t new_first_numerator = first._numerator * second._denominator;
    boost::multiprecision::int1024_t new_second_numerator = second._numerator * first._denominator;
    
    return new_first_numerator > new_second_numerator;
}

bool operator>=(
    bolshaya_drob const& first, 
    bolshaya_drob const& second) 
{
    return !(first < second);
}

bolshaya_drob bolshaya_drob::PI(
    const bolshaya_drob& EPS)
{
    if (_pi_cache.find(EPS) != _pi_cache.cend()) 
    {
        return _pi_cache[EPS];
    }
    
    static const bolshaya_drob one_fifth(1, 5);
    static const bolshaya_drob one_239(1, 239);

    auto future_arctg_1_5 = std::async(
        std::launch::async, [&]() { return arctg(one_fifth, const_cast<bolshaya_drob&>(EPS) / 16); });
    
    auto future_arctg_1_239 =
        std::async(std::launch::async, [&]() { return arctg(one_239, const_cast<bolshaya_drob&>(EPS) / 4); });

    const bolshaya_drob arctg_1_5 = future_arctg_1_5.get();
    const bolshaya_drob arctg_1_239 = future_arctg_1_239.get();

    bolshaya_drob pi = bolshaya_drob(16) * arctg_1_5 - bolshaya_drob(4) * arctg_1_239;
    _pi_cache[EPS] = pi;
   
    return pi;
}

bolshaya_drob bolshaya_drob::reduce_argument(
    const bolshaya_drob& number,
    const bolshaya_drob& EPS) 
{
    constexpr size_t reducing_threshold = 7;
    bolshaya_drob reduced_x = number;

    if (reduced_x > reducing_threshold) 
    {
        bolshaya_drob const two_pi = bolshaya_drob::PI(const_cast<bolshaya_drob&>(EPS) * 2) * 2;
        bolshaya_drob k = (const_cast<bolshaya_drob&>(number) / two_pi).truncate();
        reduced_x = const_cast<bolshaya_drob&>(number) - (k * two_pi);

        if (reduced_x > two_pi) 
        {
            reduced_x -= two_pi;
        }
        else if (reduced_x < -two_pi) 
        {
            reduced_x += two_pi;
        }
    }

    return reduced_x;
}

bolshaya_drob sin(
    bolshaya_drob const& number, 
    bolshaya_drob const& EPS) 
{
    bolshaya_drob reduced_x = bolshaya_drob::reduce_argument(number, EPS);
    bolshaya_drob res = 0;
    bolshaya_drob term = reduced_x;
    bolshaya_drob x_squared = reduced_x * reduced_x;
    boost::multiprecision::int1024_t n = 1;

    while (term.abs() > EPS) 
    {
        res += term;
        term *= -x_squared;
        ++n;
        term /= (2 * n - 1) * (2 * n - 2);
    }

    return res;
}

bolshaya_drob cos(
    bolshaya_drob const& number, 
    bolshaya_drob const& EPS) 
{
    bolshaya_drob reduced_x = bolshaya_drob::reduce_argument(number, EPS);
    bolshaya_drob res = 0;
    bolshaya_drob term = 1;
    bolshaya_drob number_squared = reduced_x * reduced_x;
    
    boost::multiprecision::int1024_t n = 0;

    while (term.abs() > EPS) 
    {
        res += term;
        term *= -number_squared;
        ++n;
        term /= (2 * n - 1) * (2 * n);
    }

    return res;
}

bolshaya_drob tg(
    const bolshaya_drob& number,
    const bolshaya_drob& EPS) 
{
    bolshaya_drob pi = bolshaya_drob::PI(EPS);
    bolshaya_drob two(2);
    bolshaya_drob half_pi = pi / two;
    bolshaya_drob x = number;
    bool negate = false;

    if (x.abs() > pi) 
    {
        x = x - (x / pi).truncate() * pi;
    }

    if (x > half_pi) 
    {
        x = pi - x;
        negate = !negate;
        x.simplify();
    }
    else if (x <= -half_pi)
    {
        x = pi + x;
        negate = !negate;
        x.simplify();
    }
    if (x < bolshaya_drob(0)) 
    {
        x = -x;
        negate = !negate;
        x.simplify();
    }

    if (x.abs() > half_pi - EPS) 
    {
        throw std::invalid_argument("Arg is too close to pi/2");
    }

    bolshaya_drob result(0);
    bolshaya_drob term;
    bolshaya_drob x_squared = x * x;
    bolshaya_drob x_power = x;

    bolshaya_drob pow_neg4_n = bolshaya_drob(-4);
    bolshaya_drob pow_4_n = bolshaya_drob(4);

    bolshaya_drob factorial = 1;
    size_t n = 1;

    for (; n <= 1000; ++n)
    {
        const bolshaya_drob& B = bolshaya_drob::Bernoulli_number(2 * n);
        bolshaya_drob coef = const_cast<bolshaya_drob&>(B) * pow_neg4_n * (bolshaya_drob(1) - pow_4_n);

        factorial *= (2 * static_cast<int>(n) - 1) * (2 * static_cast<int>(n));

        term = coef * x_power / factorial;

        if (term.abs() < EPS) 
        {
            break;
        }

        result += term;

        x_power *= x_squared;
        pow_neg4_n *= bolshaya_drob(-4);
        pow_4_n *= bolshaya_drob(4);
    }

    result.simplify();
   
    return negate ? -result : result;
}

bolshaya_drob ctg(
    bolshaya_drob const& number, 
    bolshaya_drob const& EPS) 
{
    return tg(number, EPS).reciprocal();
}

bolshaya_drob sec(
    bolshaya_drob const& number, 
    bolshaya_drob const& EPS) 
{
    return sin(number, EPS).reciprocal();
}

bolshaya_drob cosec(
    bolshaya_drob const& number, 
    bolshaya_drob const& EPS) 
{
    return cos(number, EPS).reciprocal();
}

bolshaya_drob arcsin(
    const bolshaya_drob& number, 
    const bolshaya_drob& EPS)
{
    if (number.abs() > 1) 
    {
        throw std::domain_error("arcsin is undefined for |x| > 1");
    }

    if (number.abs() > bolshaya_drob(8, 10)) 
    {
        const bolshaya_drob half_pi = bolshaya_drob::PI(EPS) / 2;
        bolshaya_drob sign = (number > 0) ? 1 : -1;
        
        return sign * (const_cast<bolshaya_drob&>(half_pi) - sqrt(arcsin((bolshaya_drob(1) - const_cast<bolshaya_drob&>(number) * const_cast<bolshaya_drob&>(number)), EPS), EPS));
    }

    bolshaya_drob term = number;
    bolshaya_drob result = term;
    bolshaya_drob number_squared = const_cast<bolshaya_drob&>(number) * const_cast<bolshaya_drob&>(number);
    boost::multiprecision::int1024_t n = 1;

    do 
    {
        term *=
            number_squared * (2 * n - 1) * (2 * n - 1) / ((2 * n) * (2 * n + 1));
        result += term;
        n++;
    } 
    while (term.abs() >= EPS);

    return result;
}

bolshaya_drob arccos(
    bolshaya_drob const& number, 
    bolshaya_drob const& EPS) 
{
    if (number.abs() > 1) 
    {
        throw std::domain_error("arccos is undefined for |x| > 1");
    }

    return bolshaya_drob::PI() / 2 - arcsin(number, EPS);
}

bolshaya_drob arcctg(
    bolshaya_drob const& number, 
    bolshaya_drob const& EPS) 
{
    if (number.abs() <= 1) 
    {
        return arctg(number.reciprocal(), EPS);
    }

    bolshaya_drob reduced_x = bolshaya_drob::reduce_argument(number, EPS);

    bolshaya_drob term = reduced_x.reciprocal();
    bolshaya_drob result = term;
    bolshaya_drob x_squared_inv = term * term;
    boost::multiprecision::int1024_t n = 1;
    int sign = -1;

    do 
    {
        term *= x_squared_inv;
        result += term * sign / (2 * n + 1);
        sign = -sign;
        n++;
    } 
    while (term.abs() >= EPS);

    return result;
}

bolshaya_drob arctg(
    bolshaya_drob const& number, 
    bolshaya_drob const& EPS) 
{
    bolshaya_drob reduced_x = bolshaya_drob::reduce_argument(number, EPS);
    
    if (reduced_x.abs() > 1) 
    {
        const bolshaya_drob half_pi = bolshaya_drob::PI(EPS) / 2;
       
        return (reduced_x > 0) ? const_cast<bolshaya_drob&>(half_pi) - arctg(reduced_x.reciprocal(), EPS)
            : -half_pi - arctg(reduced_x.reciprocal(), EPS);
    }
    if (reduced_x == 1)
    {
        return bolshaya_drob::PI(EPS) / 4;
    }

    bolshaya_drob term = reduced_x;
    bolshaya_drob result = term;
    bolshaya_drob number_squared = reduced_x * reduced_x;
    boost::multiprecision::int1024_t n = 1;
    int sign = -1;

    do 
    {
        term *= number_squared;
        result += term * sign / (2 * n + 1);
        sign = -sign;
        n++;
    } 
    while (term.abs() >= EPS);

    return result;
}

bolshaya_drob arcsec(
    bolshaya_drob const& number,
    bolshaya_drob const& EPS)
{
    return arccos(number.reciprocal(), EPS);
}

bolshaya_drob arccosec(
    bolshaya_drob const& number,
    bolshaya_drob const& EPS) 
{
    return arcsin(number.reciprocal(), EPS);
}

const bolshaya_drob& bolshaya_drob::Bernoulli_number(
    size_t n) 
{
    if (n >= _Bernoulli_cache.size()) 
    {
        calculate_Bernoulli_number_up_to(n);
    }
    
    return _Bernoulli_cache[n];
}

void bolshaya_drob::calculate_Bernoulli_number_up_to(
    size_t n) 
{
    while (_Bernoulli_cache.size() <= n) 
    {
        size_t m = _Bernoulli_cache.size();

        if (m % 2 == 1 && m > 1) 
        {
            _Bernoulli_cache.emplace_back(0);
           
            continue;
        }

        bolshaya_drob sum(0);
        
        for (size_t k = 0; k < m; ++k)
        {
            bolshaya_drob coef = binomial(m + 1, k);
            sum += coef * _Bernoulli_cache[k];
        }

        bolshaya_drob Bm = -sum / bolshaya_drob(static_cast<int>(m) + 1);
        _Bernoulli_cache.push_back(Bm);
    }
}

bolshaya_drob bolshaya_drob::binomial(
    size_t n, 
    size_t k) 
{
    if (k == 0 || k == n) 
    {
        return 1;
    }
   
    if (k > n) 
    {
        return 0;
    }

    bolshaya_drob res = 1;
   
    for (size_t i = 1; i <= k; ++i) 
    {
        res *= bolshaya_drob(static_cast<int>(n - k + i));
        res /= bolshaya_drob(static_cast<int>(i));
    }
   
    return res;
}