#include "logical_values_array.h"

unsigned int logical_values_array::accessor() const
{
	return _value;
}

logical_values_array logical_values_array::operator~() const
{
	return logical_values_array(~_value);
}

logical_values_array logical_values_array::operator&(logical_values_array const& other) const
{
	return logical_values_array(_value & other._value);
}

logical_values_array logical_values_array::operator|(logical_values_array const& other) const
{
	return logical_values_array(_value | other._value);
}

logical_values_array logical_values_array::operator^(logical_values_array const& other) const
{
	return logical_values_array(_value ^ other._value);
}

logical_values_array logical_values_array::implication(logical_values_array const& other) const
{
	return logical_values_array((~_value) | other._value);
}

logical_values_array logical_values_array::coimplication(logical_values_array const& other) const
{
	return logical_values_array(~((~_value) | other._value));
}

logical_values_array logical_values_array::equivalence(logical_values_array const& other) const
{
	return logical_values_array(~(_value ^ other._value));
}

logical_values_array logical_values_array::pierce_arrow(logical_values_array const& other) const
{
	return logical_values_array(~(_value | other._value));
}

logical_values_array logical_values_array::sheffer_stroke(logical_values_array const& other) const
{
	return logical_values_array(~(_value & other._value));
}

bool logical_values_array::equals(
	logical_values_array const& a,
	logical_values_array const& b)
{
	return a._value == b._value;
}

bool logical_values_array::get_bit(size_t const pos) const
{
	if (pos > (sizeof(unsigned int) * 8) - 1) throw std::invalid_argument("Pos argument is invalid!");

	return (_value >> pos) & 1;
}

bool logical_values_array::operator[](size_t const pos) const
{
	return get_bit(pos);
}

void logical_values_array::to_binary_string(char* dest) const
{
	if (!dest) throw std::invalid_argument("Dest argument is nullptr");

	for (int i = (sizeof(unsigned int) * 8) - 1; i >= 0; --i)
	{
		dest[sizeof(unsigned int) * 8 - i - 1] = (_value & (1 << i))
			? '1'
			: '0';
	}
	
	dest[sizeof(unsigned int) * 8] = 0;
}

logical_values_array::logical_values_array(unsigned int value = 0)
{
	_value = value;
}

logical_values_array::logical_values_array(logical_values_array const& src) = default;

logical_values_array& logical_values_array::operator=(logical_values_array const& src) = default;

logical_values_array::~logical_values_array() noexcept = default;