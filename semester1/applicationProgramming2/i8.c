#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define INVALID_BASE 1
#define NUMBER_IS_NOT_IN_BASE 2
#define NULL_PTR 3
#define MEMORY_ALLOCATION_ERROR 4
#define INVALID_COUNT_OF_NUMBERS 5

int _ctoi(
	char num,
	int const base,
	int* result_placement)
{
	if (!((isdigit(num) && num - '0' < base) ||
		(isalpha(num) && tolower(num) - 'a' < base - 10)))
	{
		return NUMBER_IS_NOT_IN_BASE;
	}

	if (base < 2 || base > 36)
	{
		return INVALID_BASE;
	}

	if (!result_placement)
	{
		return NULL_PTR;
	}

	*result_placement = (isdigit(num))
		? num - '0'
		: toupper(num) - 'A' + 10;

	return EXIT_SUCCESS;
}

int _itoc(
	int const num,
	int const base,
	char* result_placement)
{
	if (num < 0 || num >= base)
	{
		return NUMBER_IS_NOT_IN_BASE;
	}

	if (base < 2 || base > 36)
	{
		return INVALID_BASE;
	}

	if (!result_placement)
	{
		return NULL_PTR;
	}

	*result_placement = (num < 10)
		? '0' + num
		: 'A' + num - 10;

	return EXIT_SUCCESS;
}

int delete_top_zeros(char* number)
{
	int count_of_zeros = 0;
	size_t length;
	char* temp = number;

	if (!number)
	{
		return NULL_PTR;
	}

	while (*temp == '0')
	{
		++count_of_zeros;
		++temp;
	}

	length = strlen(number);

	if (count_of_zeros) 
	{
		memmove(number, number + count_of_zeros, length - count_of_zeros + 1);
	}

	return EXIT_SUCCESS;
}

int add_two_numbers(
	char const* first_number,
	char const* second_number,
	int const base,
	char** result_placement)
{
	int i, j, k, carry = 0, status, first_digit, second_digit, sum;
	size_t first_number_length, second_number_length, max_length;
	char* temp, ch;

	if (!first_number || !second_number || !result_placement)
	{
		return NULL_PTR;
	}

	if (base < 2 || base > 36)
	{
		return INVALID_BASE;
	}

	first_number_length = strlen(first_number);
	second_number_length = strlen(second_number);
	max_length = (first_number_length > second_number_length)
		? first_number_length
		: second_number_length;

	temp = (char*)calloc((max_length + 2), sizeof(char));

	if (!temp)
	{
		return MEMORY_ALLOCATION_ERROR;
	}

	temp[max_length + 1] = 0;

	i = first_number_length - 1;
	j = second_number_length - 1;
	k = max_length;

	while (i >= 0 || j >= 0)
	{
		if (i >= 0)
		{
			status = _ctoi(first_number[i--], base, &first_digit);

			if (status)
			{
				free(temp);

				return status;
			}
		}
		else
		{
			first_digit = 0;
		}

		if (j >= 0)
		{
			status = _ctoi(second_number[j--], base, &second_digit);

			if (status)
			{
				free(temp);

				return status;
			}
		}
		else
		{
			second_digit = 0;
		}

		sum = first_digit + second_digit + carry;
		carry = sum / base;

		status = _itoc(sum % base, base, &ch);
		temp[k--] = ch;

		if (status)
		{
			free(temp);

			return status;
		}
	}

	if (carry)
	{
		temp[0] = '1';
	}
	else
	{
		memmove(temp, temp + 1, max_length + 1);
	}

	if (*result_placement)
	{
		free(*result_placement);
	}

	*result_placement = temp;

	return EXIT_SUCCESS;
}

int column_multiplication_in_base(
	char** result_placement,
	int const base,
	size_t count_of_numbers,
	...)
{
	int i, status;
	va_list args;
	char const* number = NULL, * temp = NULL;

	if (base < 2 || base > 36)
	{
		return INVALID_BASE;
	}

	if (!result_placement)
	{
		return NULL_PTR;
	}

	if (count_of_numbers == 0)
	{
		return INVALID_COUNT_OF_NUMBERS;
	}

	va_start(args, count_of_numbers);

	if (count_of_numbers == 1)
	{
		number = va_arg(args, char const*);
		temp = number;

		while (*temp)
		{
			if (_ctoi(*temp, base, &i))
			{
				va_end(args);
				return NUMBER_IS_NOT_IN_BASE;
			}

			++temp;
		}

		*result_placement = (char*)malloc((strlen(number) + 1) * sizeof(char));

		if (!*result_placement)
		{
			va_end(args);
			return MEMORY_ALLOCATION_ERROR;
		}

		strcpy(*result_placement, number);

		va_end(args);

		return EXIT_SUCCESS;
	}

	number = va_arg(args, char const*);
	temp = va_arg(args, char const*);

	status = add_two_numbers(number, temp, base, result_placement);

	if (status)
	{
		va_end(args);

		return status;
	}

	for (i = 2; i < count_of_numbers; ++i)
	{
		number = va_arg(args, char const*);

		status = add_two_numbers(*result_placement, number, base, result_placement);

		if (status)
		{
			free(*result_placement);

			*result_placement = NULL;

			va_end(args);

			return status;
		}
	}

	va_end(args);

	status = delete_top_zeros(*result_placement);

	if (status)
	{
		free(*result_placement);

		return status;
	}

	return EXIT_SUCCESS;
}

int main()
{
	char* sum = NULL;

	switch (column_multiplication_in_base(&sum, 10, 2, "0000000", "1"))
	{
	case EXIT_SUCCESS:
		printf("Sum = %s\n", sum);

		break;

	case INVALID_BASE:
		printf("Invalid base!\n");

		break;

	case NUMBER_IS_NOT_IN_BASE:
		printf("Number is not in base!!\n");

		break;

	case NULL_PTR:
		printf("Pointer is NULL somewhere ._.\n");

		break;

	case MEMORY_ALLOCATION_ERROR:
		printf("Memory allocation error!!!\n");

		break;

	case INVALID_COUNT_OF_NUMBERS:
		printf("Count of numbers value cant be 0!\n");

		break;

	default:
		printf("Undefined behavior 000___ooo\n");

		break;
	}

	free(sum);

	return EXIT_SUCCESS;
}