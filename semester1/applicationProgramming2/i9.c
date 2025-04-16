#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define NULL_PTR 1
#define MEMORY_ALLOCATION_ERROR 2
#define INVALID_BASE_VALUE 3
#define UNKNOWN_FLAG 4
#define NUMBER_IS_NOT_IN_BASE 5

void my_strrev(char* str)
{
	int i;
	size_t j;
	char temp;

	for (i = 0, j = strlen(str) - 1; i < j; ++i, --j)
	{
		temp = str[j];
		str[j] = str[i];
		str[i] = temp;
	}
}

int _itor(                        // int to roman (Kalashnikov) ._.
	unsigned int number,
	char* result_placement)
{
	int i = 0;
	size_t length = 0, symbol_length;
	struct
	{
		unsigned int value;
		const char* symbol;
	} roman_digits[] = { {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
						  {100, "C"},  {90, "XC"},  {50, "L"},  {40, "XL"},
						  {10, "X"},   {9, "IX"},   {5, "V"},   {4, "IV"},
						  {1, "I"} };

	if (!result_placement)
	{
		return NULL_PTR;
	}

	if (number == 0)
	{
		*result_placement = 0;

		return EXIT_SUCCESS;
	}

	while (number > 0)
	{
		while (number >= roman_digits[i].value)
		{
			symbol_length = strlen(roman_digits[i].symbol);

			memcpy(result_placement + length, roman_digits[i].symbol, symbol_length);

			length += symbol_length;
			number -= roman_digits[i].value;
		}

		++i;
	}

	result_placement[length] = 0;

	return EXIT_SUCCESS;
}

int _lftoa(                       // long float to ASCII ._.
	long float num,
	char* buf)
{
	int is_negative = 0, status;
	long long ipart;
	long float fpart;
	size_t length;

	if (!buf)
	{
		return NULL_PTR;
	}

	if (num < 0)
	{
		is_negative = 1;
		num = -num;
	}

	ipart = (long long)num;
	fpart = num - (long float)ipart;

	if (is_negative)
	{
		*buf = '-';
	}

	status = inverse_gorner(ipart, 10, (is_negative) ? buf + 1 : buf);

	if (status)
	{
		return status;
	}

	length = strlen(buf);
	buf[length] = '.';

	fpart = fpart * pow(10, 6);

	status = inverse_gorner((long long)fpart, 10, buf + length + 1);

	if (status)
	{
		return status;
	}

	return EXIT_SUCCESS;
}

int _ftoa(                        // float to ASCII ._.
	float num,
	char* buf)
{
	int is_negative = 0, ipart, status;
	float fpart;
	size_t length;

	if (!buf)
	{
		return NULL_PTR;
	}

	if (num < 0)
	{
		is_negative = 1;
		num = -num;
	}

	ipart = (int)num;
	fpart = num - (float)ipart;

	if (is_negative)
	{
		*buf = '-';
	}

	status = inverse_gorner(ipart, 10, (is_negative) ? buf + 1 : buf);

	if (status)
	{
		return status;
	}

	length = strlen(buf);
	buf[length] = '.';

	fpart = fpart * pow(10, 6);

	status = inverse_gorner((int)fpart, 10, buf + length + 1);

	if (status)
	{
		return status;
	}
	
	return EXIT_SUCCESS;
}

int gorner(
	char const* str_num,
	unsigned int const base,
	char * result_placement)
{
	unsigned int result = 0;

	if (result_placement == NULL)
	{
		return NULL_PTR;
	}

	if (str_num == NULL || !*str_num)
	{
		*result_placement = 0;

		return EXIT_SUCCESS;
	}

	if (base < 2 || base > 36)
	{
		return INVALID_BASE_VALUE;
	}

	while (*str_num)
	{
		if (!((isdigit(*str_num) && *str_num - '0' < base) ||
			(isalpha(*str_num) && toupper(*str_num) - 'A' < base - 10)))
		{
			return NUMBER_IS_NOT_IN_BASE;
		}

		result = result * base + (isdigit(*str_num)
			? *str_num - '0'
			: toupper(*str_num) - 'A' + 10);

		++str_num;
	}

	inverse_gorner_for_uint(result, 10, result_placement);

	return EXIT_SUCCESS;
}

int inverse_gorner_for_uint_lowercased(
	unsigned int num,
	unsigned int const base,
	char* result_placement)
{
	unsigned int i = 0;
	int digit;

	if (base < 2 || base > 36)
	{
		return INVALID_BASE_VALUE;
	}

	if (result_placement == NULL)
	{
		return NULL_PTR;
	}

	do
	{
		digit = num % base;
		result_placement[i++] = (digit < 10)
			? digit + '0'
			: digit - 10 + 'a';
		num /= base;
	} while (num > 0);

	result_placement[i] = 0;

	my_strrev(result_placement);

	return EXIT_SUCCESS;
}

int inverse_gorner_for_uint(
	unsigned int num,
	unsigned int const base,
	char* result_placement)
{
	unsigned int i = 0;
	int digit;

	if (base < 2 || base > 36)
	{
		return INVALID_BASE_VALUE;
	}

	if (result_placement == NULL)
	{
		return NULL_PTR;
	}

	do
	{
		digit = num % base;
		result_placement[i++] = (digit < 10)
			? digit + '0'
			: digit - 10 + 'A';
		num /= base;
	} while (num > 0);

	result_placement[i] = 0;

	my_strrev(result_placement);

	return EXIT_SUCCESS;
}

int inverse_gorner(
	int num,
	unsigned int const base,
	char* result_placement)
{
	int i = 0, digit, is_negative = 0;

	if (base < 2 || base > 36)
	{
		return INVALID_BASE_VALUE;
	}

	if (result_placement == NULL)
	{
		return NULL_PTR;
	}

	if (num < 0)
	{
		is_negative = 1;
		num = -num;
	}

	do
	{
		digit = num % base;
		result_placement[i++] = (digit < 10)
			? digit + '0'
			: digit - 10 + 'A';
		num /= base;
	} while (num > 0);

	if (is_negative)
	{
		result_placement[i++] = '-';
	}

	result_placement[i] = 0;

	my_strrev(result_placement);

	return EXIT_SUCCESS;
}

int float_memory_dump(
	float value,
	char* result_placement)
{
	unsigned char* byte_pointer = (unsigned char*)&value;
	int i, j;
	size_t offset = 0;

	if (!result_placement) return NULL_PTR;

	for (i = 0; i < sizeof(float); i++)
	{
		for (j = 7; j >= 0; j--)
		{
			result_placement[offset++] = ((byte_pointer[i] >> j) & 1) + '0';
		}
		if (i < sizeof(float) - 1)
		{
			result_placement[offset++] = ' ';
		}
	}

	result_placement[offset] = 0;

	return EXIT_SUCCESS;
}

int double_memory_dump(
	double value,
	char* result_placement)
{
	unsigned char* byte_pointer = (unsigned char*)&value;
	int i, j;
	size_t offset = 0;

	if (!result_placement) return NULL_PTR;

	for (i = 0; i < sizeof(double); i++)
	{
		for (j = 7; j >= 0; j--)
		{
			result_placement[offset++] = ((byte_pointer[i] >> j) & 1) + '0';
		}
		if (i < sizeof(double) - 1)
		{
			result_placement[offset++] = ' ';
		}
	}

	result_placement[offset] = 0;

	return EXIT_SUCCESS;
}

int uint_memory_dump(
	unsigned int value,
	char* result_placement)
{
	unsigned char* byte_pointer = (unsigned char*)&value;
	int i, j;
	size_t offset = 0;

	if (!result_placement) return NULL_PTR;

	for (i = 0; i < sizeof(unsigned int); i++)
	{
		for (j = 7; j >= 0; j--)
		{
			result_placement[offset++] = ((byte_pointer[i] >> j) & 1) + '0';
		}
		if (i < sizeof(unsigned int) - 1)
		{
			result_placement[offset++] = ' ';
		}
	}
	
	result_placement[offset] = 0;

	return EXIT_SUCCESS;
}

int int_memory_dump(
	int value,
	char* result_placement)
{
	unsigned char* byte_pointer = (unsigned char*)&value;
	int i, j;
	size_t offset = 0;

	if (!result_placement) return NULL_PTR;

	for (i = 0; i < sizeof(int); i++)
	{
		for (j = 7; j >= 0; j--)
		{
			result_placement[offset++] = ((byte_pointer[i] >> j) & 1) + '0';
		}
		if (i < sizeof(int) - 1)
		{
			result_placement[offset++] = ' ';
		}
	}
	
	result_placement[offset] = 0;

	return EXIT_SUCCESS;
}

void generate_Fibonacci_sequence(
	int number,
	int fib[],
	size_t* size_of_sequence)
{
	int next;

	fib[0] = 1;
	fib[1] = 2;

	*size_of_sequence = 2;

	while (1)
	{
		next = fib[*size_of_sequence - 1] + fib[*size_of_sequence - 2];

		if (next > number) break;

		fib[*size_of_sequence] = next;
		(*size_of_sequence)++;
	}

}

int to_find_Zeckendorf_representation(
	unsigned int number,
	char* result_placement)
{
	int fib[50], i, counter = 0, status;
	size_t size_of_sequence, num_length, length = 0;
	char num_buf[32];

	if (!result_placement)
	{
		return NULL_PTR;
	}

	if (number == 0)
	{
		*result_placement = 0;

		return EXIT_SUCCESS;
	}

	generate_Fibonacci_sequence(number, fib, &size_of_sequence);

	while (number > 0)
	{
		for (i = size_of_sequence - 1; i >= 0; --i)
		{
			if (fib[i] <= number)
			{
				status = inverse_gorner_for_uint(fib[i], 10, num_buf);

				if (status) return status;

				num_length = strlen(num_buf);

				memcpy(result_placement + length, num_buf, num_length);

				length += num_length;
				result_placement[length++] = ' ';
				result_placement[length++] = '+';
				result_placement[length++] = ' ';
				number -= fib[i];
			}
		}
	}

	result_placement[length - 3] = 0;

	return EXIT_SUCCESS;
}

int formatting_logic(
	char **buffer,
	char const* format,
	va_list args)
{
	int current_format_index = 0, current_buffer_index = 0, capacity = BUFSIZ, status;
	size_t size_of_something;
	char* for_reallocation_ptr, number_buffer[BUFSIZ], *str_ptr, *num_ptr;
	unsigned int num, base;

	if (!format)
	{
		return EXIT_SUCCESS;
	}

	*buffer = (char*)malloc(capacity * sizeof(char));

	if (!*buffer)
	{
		return MEMORY_ALLOCATION_ERROR;
	}

	while (format[current_format_index] != 0)
	{
		if (format[current_format_index] == '%')
		{
			++current_format_index;

			switch (format[current_format_index])
			{
			case '%':
				(*buffer)[current_buffer_index++] = '%';

				if (current_buffer_index + 1 > capacity - 1)
				{
					capacity *= 2;
					for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

					if (!for_reallocation_ptr)
					{
						free(*buffer);

						return MEMORY_ALLOCATION_ERROR;
					}

					*buffer = for_reallocation_ptr;
				}

				break; 

			case 'd':
				status = inverse_gorner(va_arg(args, int), 10, number_buffer);

				if (status)
				{
					free(*buffer);

					return status;
				}

				size_of_something = strlen(number_buffer);

				if (current_buffer_index + size_of_something > capacity - 1)
				{
					capacity *= 2;
					for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

					if (!for_reallocation_ptr)
					{
						free(*buffer);

						return MEMORY_ALLOCATION_ERROR;
					}

					*buffer = for_reallocation_ptr;
				}

				memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

				current_buffer_index += size_of_something;
				*number_buffer = 0;

				break;

			case 'u':
				status = inverse_gorner_for_uint(va_arg(args, unsigned int), 10, number_buffer);

				if (status)
				{
					free(*buffer);

					return status;
				}

				size_of_something = strlen(number_buffer);

				if (current_buffer_index + size_of_something > capacity - 1)
				{
					capacity *= 2;
					for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

					if (!for_reallocation_ptr)
					{
						free(*buffer);

						return MEMORY_ALLOCATION_ERROR;
					}

					*buffer = for_reallocation_ptr;
				}

				memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

				current_buffer_index += size_of_something;
				*number_buffer = 0;

				break;

			case 'f':
				status = _ftoa(va_arg(args, double), number_buffer);

				if (status)
				{
					free(*buffer);

					return status;
				}

				size_of_something = strlen(number_buffer);

				if (current_buffer_index + size_of_something > capacity - 1)
				{
					capacity *= 2;
					for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

					if (!for_reallocation_ptr)
					{
						free(*buffer);

						return MEMORY_ALLOCATION_ERROR;
					}

					*buffer = for_reallocation_ptr;
				}

				memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

				current_buffer_index += size_of_something;
				*number_buffer = 0;

				break;

			case 'l':
				++current_format_index;

				switch (format[current_format_index])
				{
				case 'f':
					status = _lftoa(va_arg(args, long float), number_buffer);

					if (status)
					{
						free(*buffer);

						return status;
					}

					size_of_something = strlen(number_buffer);

					if (current_buffer_index + size_of_something > capacity - 1)
					{
						capacity *= 2;
						for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

						if (!for_reallocation_ptr)
						{
							free(*buffer);

							return MEMORY_ALLOCATION_ERROR;
						}

						*buffer = for_reallocation_ptr;
					}

					memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

					current_buffer_index += size_of_something;
					*number_buffer = 0;

					break;

				default:
					free(*buffer);

					return UNKNOWN_FLAG;
				}

				break;

			case 'c':
				(*buffer)[current_buffer_index++] = va_arg(args, char);

				if (current_buffer_index + 1 > capacity - 1)
				{
					capacity *= 2;
					for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

					if (!for_reallocation_ptr)
					{
						free(*buffer);

						return MEMORY_ALLOCATION_ERROR;
					}

					*buffer = for_reallocation_ptr;
				}

				break;

			case 's':
				str_ptr = va_arg(args, char*);
				
				if (!str_ptr)
				{
					free(*buffer);

					return NULL_PTR;
				}

				size_of_something = strlen(str_ptr);

				if (current_buffer_index + size_of_something > capacity - 1)
				{
					capacity *= 2;
					for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

					if (!for_reallocation_ptr)
					{
						free(*buffer);

						return MEMORY_ALLOCATION_ERROR;
					}

					*buffer = for_reallocation_ptr;
				}

				memcpy(*buffer + current_buffer_index, str_ptr, size_of_something);

				current_buffer_index += size_of_something;
				str_ptr = NULL;

				break;

			case 'o':
				status = inverse_gorner_for_uint(va_arg(args, unsigned int), 8, number_buffer);

				if (status)
				{
					free(*buffer);

					return status;
				}

				size_of_something = strlen(number_buffer);

				if (current_buffer_index + size_of_something > capacity - 1)
				{
					capacity *= 2;
					for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

					if (!for_reallocation_ptr)
					{
						free(*buffer);

						return MEMORY_ALLOCATION_ERROR;
					}

					*buffer = for_reallocation_ptr;
				}

				memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

				current_buffer_index += size_of_something;
				*number_buffer = 0;

				break;

			case 'x':
				status = inverse_gorner_for_uint_lowercased(va_arg(args, unsigned int), 16, number_buffer);

				if (status) 
				{
					free(*buffer);

					return status;
				}

				size_of_something = strlen(number_buffer);

				if (current_buffer_index + size_of_something > capacity - 1)
				{
					capacity *= 2;
					for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

					if (!for_reallocation_ptr)
					{
						free(*buffer);

						return MEMORY_ALLOCATION_ERROR;
					}

					*buffer = for_reallocation_ptr;
				}

				memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

				current_buffer_index += size_of_something;
				*number_buffer = 0;

				break;

			case 'X':
				status = inverse_gorner_for_uint(va_arg(args, unsigned int), 16, number_buffer);

				if (status)
				{
					free(*buffer);

					return status;
				}

				size_of_something = strlen(number_buffer);

				if (current_buffer_index + size_of_something > capacity - 1)
				{
					capacity *= 2;
					for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

					if (!for_reallocation_ptr)
					{
						free(*buffer);

						return MEMORY_ALLOCATION_ERROR;
					}

					*buffer = for_reallocation_ptr;
				}

				memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

				current_buffer_index += size_of_something;
				*number_buffer = 0;

				break;

			case 'R':
				++current_format_index;

				switch (format[current_format_index])
				{
				case 'o':
					status = _itor(va_arg(args, unsigned int), number_buffer);

					if (status)
					{
						free(*buffer);

						return status;
					}

					size_of_something = strlen(number_buffer);

					if (current_buffer_index + size_of_something > capacity - 1)
					{
						capacity *= 2;
						for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

						if (!for_reallocation_ptr)
						{
							free(*buffer);

							return MEMORY_ALLOCATION_ERROR;
						}

						*buffer = for_reallocation_ptr;
					}

					memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

					current_buffer_index += size_of_something;
					*number_buffer = 0;

					break;

				default:
					free(*buffer);

					return UNKNOWN_FLAG;
				}

				break;

			case 'Z':
				++current_format_index;

				switch (format[current_format_index])
				{
				case 'r':
					status = to_find_Zeckendorf_representation(va_arg(args, unsigned int), number_buffer);

					if (status)
					{
						free(*buffer);

						return status;
					}

					size_of_something = strlen(number_buffer);

					if (current_buffer_index + size_of_something > capacity - 1)
					{
						capacity *= 2;
						for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

						if (!for_reallocation_ptr)
						{
							free(*buffer);

							return MEMORY_ALLOCATION_ERROR;
						}

						*buffer = for_reallocation_ptr;
					}

					memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

					current_buffer_index += size_of_something;
					*number_buffer = 0;

					break;

				default:
					free(*buffer);

					return UNKNOWN_FLAG;
				}

				break;

			case 'C':
				++current_format_index;

				switch (format[current_format_index])
				{
				case 'v':
					num = va_arg(args, unsigned int);
					base = va_arg(args, unsigned int);

					if (base < 2 || base > 36) base = 10;

					status = inverse_gorner_for_uint_lowercased(num, base, number_buffer);

					if (status)
					{
						free(*buffer);

						return status;
					}

					size_of_something = strlen(number_buffer);

					if (current_buffer_index + size_of_something > capacity - 1)
					{
						capacity *= 2;
						for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

						if (!for_reallocation_ptr)
						{
							free(*buffer);

							return MEMORY_ALLOCATION_ERROR;
						}

						*buffer = for_reallocation_ptr;
					}

					memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

					current_buffer_index += size_of_something;
					*number_buffer = 0;

					break;

				case 'V':
					num = va_arg(args, unsigned int);
					base = va_arg(args, unsigned int);

					if (base < 2 || base > 36) base = 10;

					status = inverse_gorner_for_uint(num, base, number_buffer);

					size_of_something = strlen(number_buffer);

					if (current_buffer_index + size_of_something > capacity - 1)
					{
						capacity *= 2;
						for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

						if (!for_reallocation_ptr)
						{
							free(*buffer);

							return MEMORY_ALLOCATION_ERROR;
						}

						*buffer = for_reallocation_ptr;
					}

					memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

					current_buffer_index += size_of_something;
					*number_buffer = 0;

					break;

				default:
					free(*buffer);

					return UNKNOWN_FLAG;
				}

				break;
				
			case 't':
				++current_format_index;

				switch (format[current_format_index])
				{
				case 'o':
					num_ptr = va_arg(args, char*);
					base = va_arg(args, unsigned int);

					if (!num_ptr)
					{
						free(*buffer);

						return NULL_PTR;
					}

					if (base < 2 || base > 36) base = 10;

					status = gorner(num_ptr, base, number_buffer);

					if (status)
					{
						free(*buffer);

						return status;
					}

					size_of_something = strlen(number_buffer);

					if (current_buffer_index + size_of_something > capacity - 1)
					{
						capacity *= 2;
						for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

						if (!for_reallocation_ptr)
						{
							free(*buffer);

							return MEMORY_ALLOCATION_ERROR;
						}

						*buffer = for_reallocation_ptr;
					}

					memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

					current_buffer_index += size_of_something;
					*number_buffer = 0;

					break;

				default:
					free(*buffer);

					return UNKNOWN_FLAG;
				}

				break;

			case 'T':
				++current_format_index;

				switch (format[current_format_index])
				{
				case 'O':
					num_ptr = va_arg(args, char*);
					base = va_arg(args, unsigned int);

					if (base < 2 || base > 36) base = 10;

					status = gorner(num_ptr, base, number_buffer);

					if (status)
					{
						free(*buffer);

						return status;
					}

					size_of_something = strlen(number_buffer);

					if (current_buffer_index + size_of_something > capacity - 1)
					{
						capacity *= 2;
						for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

						if (!for_reallocation_ptr)
						{
							free(*buffer);

							return MEMORY_ALLOCATION_ERROR;
						}

						*buffer = for_reallocation_ptr;
					}

					memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

					current_buffer_index += size_of_something;
					*number_buffer = 0;

					break;

				default:
					free(*buffer);

					return UNKNOWN_FLAG;
				}

				break;

			case 'm':
				++current_format_index;

				switch (format[current_format_index])
				{
				case 'i':
					status = int_memory_dump(va_arg(args, int), number_buffer);

					if (status)
					{
						free(*buffer);

						return status;
					}

					size_of_something = strlen(number_buffer);

					if (current_buffer_index + size_of_something > capacity - 1)
					{
						capacity *= 2;
						for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

						if (!for_reallocation_ptr)
						{
							free(*buffer);

							return MEMORY_ALLOCATION_ERROR;
						}

						*buffer = for_reallocation_ptr;
					}

					memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

					current_buffer_index += size_of_something;
					*number_buffer = 0;

					break;

				case 'u':
					status = uint_memory_dump(va_arg(args, unsigned int), number_buffer);

					if (status)
					{
						free(*buffer);

						return status;
					}

					size_of_something = strlen(number_buffer);

					if (current_buffer_index + size_of_something > capacity - 1)
					{
						capacity *= 2;
						for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

						if (!for_reallocation_ptr)
						{
							free(*buffer);

							return MEMORY_ALLOCATION_ERROR;
						}

						*buffer = for_reallocation_ptr;
					}

					memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

					current_buffer_index += size_of_something;
					*number_buffer = 0;

					break;

				case 'd':
					status = double_memory_dump(va_arg(args, double), number_buffer);

					if (status)
					{
						free(*buffer);

						return status;
					}

					size_of_something = strlen(number_buffer);

					if (current_buffer_index + size_of_something > capacity - 1)
					{
						capacity *= 2;
						for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

						if (!for_reallocation_ptr)
						{
							free(*buffer);

							return MEMORY_ALLOCATION_ERROR;
						}

						*buffer = for_reallocation_ptr;
					}

					memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

					current_buffer_index += size_of_something;
					*number_buffer = 0;

					break;

				case 'f':
					status = float_memory_dump(va_arg(args, float), number_buffer);

					if (status)
					{
						free(*buffer);

						return status;
					}

					size_of_something = strlen(number_buffer);

					if (current_buffer_index + size_of_something > capacity - 1)
					{
						capacity *= 2;
						for_reallocation_ptr = (char*)realloc(*buffer, capacity * sizeof(char));

						if (!for_reallocation_ptr)
						{
							free(*buffer);

							return MEMORY_ALLOCATION_ERROR;
						}

						*buffer = for_reallocation_ptr;
					}

					memcpy(*buffer + current_buffer_index, number_buffer, size_of_something);

					current_buffer_index += size_of_something;
					*number_buffer = 0;

					break;

				default:
					free(*buffer);

					return UNKNOWN_FLAG;
				}

				break;

			default:
				free(*buffer);

				return UNKNOWN_FLAG;
			}
		}
		else
		{
			(*buffer)[current_buffer_index++] = format[current_format_index];
		}

		++current_format_index;
	}

	(*buffer)[current_buffer_index] = 0;

	return EXIT_SUCCESS;
}

int overprintf(
	char const* format,
	...)
{
	int status;
	char* buffer, *temp;
	va_list args;

	va_start(args, format);

	status = formatting_logic(&buffer, format, args);

	if (status)
	{
		va_end(args);

		return status;
	}

	temp = buffer;

	while (*temp)
	{
		fputc(*temp++, stdout);
	}

	va_end(args);
	free(buffer);

	return EXIT_SUCCESS;
}

int overfprintf(
	FILE* file,
	char const* format,
	...)
{
	int status;
	char* buffer, * temp;
	va_list args;

	va_start(args, format);

	status = formatting_logic(&buffer, format, args);

	if (status)
	{
		va_end(args);

		return status;
	}

	temp = buffer;

	while (*temp)
	{
		fputc(*temp++, file);
	}

	va_end(args);
	free(buffer);

	return EXIT_SUCCESS;
}

int oversprintf(
	char* dest,
	char const* format,
	...)
{
	int status;
	char* buffer;
	va_list args;

	va_start(args, format);

	status = formatting_logic(&buffer, format, args);

	if (status)
	{
		va_end(args);

		return status;
	}

	strcpy(dest, buffer);

	va_end(args);
	free(buffer);

	return EXIT_SUCCESS;
}

int main()
{
	overprintf("");

	return EXIT_SUCCESS;
}