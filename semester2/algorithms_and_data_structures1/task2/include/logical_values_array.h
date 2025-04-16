#ifndef LOGICAL_VALUES_ARRAY_H
#define LOGICAL_VALUES_ARRAY_H

#include <iostream>
#include <stdexcept>
#include <cstring>

class logical_values_array final
{
private:

	unsigned int _value;

public:

	unsigned int accessor() const;

	logical_values_array operator~() const;

	logical_values_array operator&(logical_values_array const& other) const;

	logical_values_array operator|(logical_values_array const& other) const;

	logical_values_array operator^(logical_values_array const& other) const;

	logical_values_array implication(logical_values_array const& other) const;

	logical_values_array coimplication(logical_values_array const& other) const;

	logical_values_array equivalence(logical_values_array const& other) const;

	logical_values_array pierce_arrow(logical_values_array const& other) const;

	logical_values_array sheffer_stroke(logical_values_array const& other) const;

	bool equals(
		logical_values_array const& a, 
		logical_values_array const& b);

	bool get_bit(size_t const pos) const;

	bool operator[](size_t const pos) const;

	void to_binary_string(char* dest) const;

	logical_values_array(unsigned int value);

	logical_values_array(logical_values_array const& src);						
																							
	logical_values_array& operator=(logical_values_array const& src);				
																							
	~logical_values_array() noexcept;												
};

#endif // LOGICAL_VALUES_ARRAY_H