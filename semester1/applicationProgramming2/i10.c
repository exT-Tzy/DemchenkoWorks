#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#pragma warning(disable: 4996)

#define NULL_PTR 1
#define UNKNOWN_STREAM_TYPE 2
#define INVALID_BASE_VALUE 3
#define NUMBER_IS_NOT_IN_BASE 4
#define UNKNOWN_FLAG 5

typedef enum
{
	STDIN_STREAM,
	FILE_STREAM,
	STRING_STREAM
} stream_type;

int _rtoi(					// roman (Kalashnikov) to int ._.
	char const* str,
	int* result_placement)
{
	size_t length;
	int i = 0, j = 0;
	char letter1, letter2;
	int temp = 0;

	struct
	{
		unsigned int value;
		size_t length;
		char* symbol;
	} roman_digits[] = {
		{1000, 1, "M"}, {900, 2, "CM"}, {500, 1, "D"}, {400, 2, "CD"},
		{100, 1, "C"},  {90, 2, "XC"},  {50, 1, "L"},  {40, 2, "XL"},
		{10, 1, "X"},   {9, 2, "IX"},   {5, 1, "V"},   {4, 2, "IV"},
		{1, 1, "I"} };

	if (!str || !*str) return EXIT_SUCCESS;
	if (!result_placement) return NULL_PTR;

	length = strlen(str);

	while (i < length)
	{
		letter1 = str[i];
		letter2 = str[i + 1];

		for (; j < 13; ++j)
		{
			if (roman_digits[j].length == 1 && roman_digits[j].symbol[0] == letter1)
			{
				temp += roman_digits[j].value;

				break;
			}
			if (roman_digits[j].length == 2 && roman_digits[j].symbol[0] == letter1 && roman_digits[j].symbol[1] == letter2)
			{
				temp += roman_digits[j].value;
				++i;

				break;
			}
		}

		++i;
	}

	*result_placement = temp;

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

int gorner_for_uint(
	char const* str_num,
	unsigned int const base,
	unsigned int* result_placement)
{
	unsigned int result = 0;

	if (result_placement == NULL)
	{
		return NULL_PTR;
	}

	if (str_num == NULL || !*str_num)
	{
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

	*result_placement = result;

	return EXIT_SUCCESS;
}

int gorner(
	char const* str_num,
	unsigned int const base,
	int* result_placement)
{
	int result = 0, is_negative = 0;

	if (result_placement == NULL)
	{
		return NULL_PTR;
	}

	if (str_num == NULL || !*str_num)
	{
		return EXIT_SUCCESS;
	}

	if (base < 2 || base > 36)
	{
		return INVALID_BASE_VALUE;
	}

	if (*str_num == '-')
	{
		is_negative = 1;
		++str_num;
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

	*result_placement = (is_negative)
		? -result
		: result;

	return EXIT_SUCCESS;
}

int scan_from_stream(
	stream_type stream,
	void** stream_ptr,
	char* dest)
{
	char c, *temp_dest_ptr = dest, *str;
	FILE** file_pp;

	if (!stream_ptr || !dest)
	{
		return NULL_PTR;
	}

	switch (stream)
	{
	case STDIN_STREAM:
		c = fgetc(stdin);

		while (c != EOF && (c == ' ' || c == '\t' || c == '\n')) c = fgetc(stdin);

		while (c != 0 && c != '\n' && c != ' ' && c != '\t')
		{
			*temp_dest_ptr++ = c;
			c = fgetc(stdin);
		}

		*temp_dest_ptr = 0;
		break;

	case FILE_STREAM:
		file_pp = (FILE**)stream_ptr;
		c = fgetc(*file_pp);

		while (c != EOF && (c == ' ' || c == '\t' || c == '\n')) c = fgetc(stdin);

		while (c != 0 && c != '\n' && c != ' ' && c != '\t' && c != EOF)
		{
			*temp_dest_ptr++ = c;
			c = fgetc(*file_pp);
		}

		*temp_dest_ptr = 0;
		break;

	case STRING_STREAM:
		str = *((char**)stream_ptr);
		c = *str;
		str++;

		while (c != 0 && (c == ' ' || c == '\n' || c == '\t'))
		{
			c = *str;
			str++;
		}

		while (c != 0 && c != '\n' && c != '\t' && c != ' ')
		{
			*temp_dest_ptr++ = c;
			c = *str;
			str++;
		}

		*temp_dest_ptr = 0;
		*stream_ptr = str;
		break;

	default:
		return UNKNOWN_STREAM_TYPE;
	}

	return EXIT_SUCCESS;
}

int scanning_logic(
	stream_type stream,
	void* stream_ptr,
	char const* format,
	va_list args)
{
	int status, current_format_index = 0, temp = 0, *int_ptr;
	unsigned int base;
	char buf1[BUFSIZ], buf2[BUFSIZ];
	

	if (!stream_ptr || !format) return NULL_PTR;
	if (!*format) return EXIT_SUCCESS;

	while (format[current_format_index] != 0)
	{
		if (format[current_format_index] == '%')
		{
			++current_format_index;

			switch (format[current_format_index])
			{
			case 'd':
				status = scan_from_stream(stream, &stream_ptr, buf1);

				if (status) return status;

				status = gorner(buf1, 10, va_arg(args, int*));

				if (status) return status;

				break;

			case 'u':
				status = scan_from_stream(stream, &stream_ptr, buf1);

				if (status) return status;

				status = gorner_for_uint(buf1, 10, va_arg(args, unsigned int*));

				if (status) return status;

				break;

			case 'f':
				status = scan_from_stream(stream, &stream_ptr, buf1);

				if (status) return status;

				*va_arg(args, double*) = atof(buf1);

				break;

			case 'l':
				++current_format_index;

				switch (format[current_format_index])
				{
				case 'f':
					status = scan_from_stream(stream, &stream_ptr, buf1);

					if (status) return status;

					*va_arg(args, double*) = atof(buf1);

					break;

				default:
					return UNKNOWN_FLAG;
				}

				break;

			case 'c':
				switch (stream)
				{
				case STDIN_STREAM:
					*va_arg(args, char*) = fgetc(stdin);

					break;

				case FILE_STREAM:
					*va_arg(args, char*) = fgetc((FILE*)stream_ptr);

					break;

				case STRING_STREAM:
					*va_arg(args, char*) = *((char*)stream_ptr);
					stream_ptr = (char*)stream_ptr + 1;

					break;
				}

				break;

			case 's':
				status = scan_from_stream(stream, &stream_ptr, buf1);

				if (status) return status;

				strcpy(va_arg(args, char*), buf1);

				break;

			case 'o':
				status = scan_from_stream(stream, &stream_ptr, buf1);

				if (status) return status;

				status = gorner(buf1, 8, va_arg(args, int*));

				if (status) return status;

				break;

			case 'x':
				status = scan_from_stream(stream, &stream_ptr, buf1);

				if (status) return status;

				status = gorner(buf1, 16, va_arg(args, int*));

				if (status) return status;

				break;

			case 'X':
				status = scan_from_stream(stream, &stream_ptr, buf1);

				if (status) return status;

				status = gorner(buf1, 16, va_arg(args, int*));

				if (status) return status;

				break;

			case 'R':
				++current_format_index;

				switch (format[current_format_index])
				{
				case 'o':
					status = scan_from_stream(stream, &stream_ptr, buf1);

					if (status) return status;

					status = _rtoi(buf1, va_arg(args, unsigned int *));

					if (status) return status;

					break;

				default:
					return UNKNOWN_FLAG;
				}

				break;

			case 'Z':
				++current_format_index;

				switch (format[current_format_index])
				{
				case 'r':
					status = scan_from_stream(stream, &stream_ptr, buf1);

					if (status) return status;

					break;

				default:
					return UNKNOWN_FLAG;
				}

				break;

			case 'C':
				++current_format_index;

				switch (format[current_format_index])
				{
				case 'v':
					status = scan_from_stream(stream, &stream_ptr, buf1);

					if (status) return status;

					int_ptr = va_arg(args, int*);
					base = va_arg(args, unsigned int);

					status = gorner(buf1, base, int_ptr);

					if (status) return status;

					break;

				case 'V':
					status = scan_from_stream(stream, &stream_ptr, buf1);

					if (status) return status;

					int_ptr = va_arg(args, int*);
					base = va_arg(args, unsigned int);

					status = gorner(buf1, base, int_ptr);

					if (status) return status;

					break;

				default:
					return UNKNOWN_FLAG;
				}

				break;

			default:
				return UNKNOWN_FLAG;
			}
		}

		++current_format_index;
	}

	return EXIT_SUCCESS;
}

int oversscanf(
	char const* str,
	char const* format,
	...)
{
	int status;
	char* str_ptr;
	va_list args;

	if (!str) return NULL_PTR;
	if (!*str) return EXIT_SUCCESS;

	va_start(args, format);

	str_ptr = str;

	status = scanning_logic(STRING_STREAM, (void*)str_ptr, format, args);

	va_end(args);

	if (status) return status;

	return EXIT_SUCCESS;
}

int overscanf(
	char const* format,
	...)
{
	int status;
	va_list args;

	if (!format) return NULL_PTR;
	if (!*format) return EXIT_SUCCESS;

	va_start(args, format);

	status = scanning_logic(STDIN_STREAM, (void*)stdin, format, args);

	va_end(args);

	if (status) return status;

	return EXIT_SUCCESS;
}

int overfscanf(
	FILE* file,
	char const *format,
	...)
{
	int status;
	va_list args;

	if (!file || !format) return NULL_PTR;
	if (!*format) return EXIT_SUCCESS;

	va_start(args, format);

	status = scanning_logic(FILE_STREAM, (void*)file, format, args);

	va_end(args);

	if (status) return status;

	return EXIT_SUCCESS;
}

int main()
{
	int a;

	overscanf("%Cv", &a, 16);

	printf("%d", a);

	return EXIT_SUCCESS;
}