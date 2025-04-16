#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

size_t my_strlen(const char* str) 
{
	size_t length = 0;

	while (str[length] != 0) 
	{
		length++;
	}

	return length;
}

void my_strcat(
	char* dest,
	const char* src) 
{
	while (*dest) 
	{
		dest++;
	}
	while (*src) 
	{
		*dest++ = *src++;
	}

	*dest = 0;
}

void my_strcpy(
	char* dest,
	const char* src) 
{
	while (*src) 
	{
		*dest++ = *src++;
	}

	*dest = 0;
}

int reverse_str(
	const char* str,
	char **result) 
{
	size_t str_symbols_count = my_strlen(str);
	*result = (char*)malloc((str_symbols_count + 1) * sizeof(char));

	if (*result == NULL) 
	{
		return 1;
	}

	int i;
	
	(*result)[str_symbols_count] = 0;

	for (i = 0; i < str_symbols_count; ++i) 
	{
		(*result)[str_symbols_count - 1 - i] = str[i];
	}

	return 0;
}

int to_uppercase_odd_positions(
	const char* str,
	char** result) 
{
	size_t str_symbols_count = my_strlen(str), i;
	
	*result = (char*)malloc((str_symbols_count + 1) * sizeof(char));

	if (*result == NULL) 
	{
		return 1;
	}

	(*result)[str_symbols_count] = 0;

	for (i = 0; i < str_symbols_count; i++) 
	{
		(*result)[i] = i % 2 == 0
			? str[i]
			: toupper(str[i]);
	}

	return 0;
}

int rearrange(
	const char* str,
	char** result) 
{
	size_t str_symbols_count = my_strlen(str);
	*result = (char*)malloc((str_symbols_count + 1) * sizeof(char));

	if (*result == NULL) 
	{
		return 1;
	}

	char* digits = (char*)malloc((str_symbols_count + 1) * sizeof(char));

	if (digits == NULL) 
	{
		return 1;
	}

	char* letters = (char*)malloc((str_symbols_count + 1) * sizeof(char));

	if (letters == NULL) 
	{
		free(digits);
		
		return 1;
	}

	char* others = (char*)malloc((str_symbols_count + 1) * sizeof(char));

	if (others == NULL) 
	{
		free(digits);
		free(letters);
		
		return 1;
	}
	
	char* digits_ptr = digits, * letters_ptr = letters, * others_ptr = others;

	while (*str) 
	{
		if (isdigit(*str)) 
		{
			*digits_ptr++ = *str;
		}
		else if (isalpha(*str)) 
		{
			*letters_ptr++ = *str;
		}
		else 
		{
			*others_ptr++ = *str;
		}
		
		str++;
	}

	*digits_ptr = 0;
	*letters_ptr = 0;
	*others_ptr = 0;

	my_strcpy(*result, digits);
	my_strcat(*result, letters);
	my_strcat(*result, others);

	free(digits);
	free(letters);
	free(others);

	return 0;
}

int concatenate_strings_with_srand(
	char** array_of_pointers_on_strings,
	char** result,
	unsigned int seed,
	size_t count_of_strings) 
{
	if (array_of_pointers_on_strings == NULL)
	{
		return 1;
	}

	size_t result_len = 0;
	int i;

	for (i = 0; i < count_of_strings; ++i)
	{
		result_len += my_strlen(array_of_pointers_on_strings[i]);
	}

	*result = (char*)malloc((result_len + 1) * sizeof(char));

	if (*result == NULL)
	{
		return 2;
	}

	int* rand_numbers = (int*)malloc(count_of_strings * sizeof(int));

	if (rand_numbers == NULL)
	{
		free(*result);

		return 2;
	}

	int j, temp;
	char* temp_ptr;

	for (i = 0; i < count_of_strings; ++i)
	{
		rand_numbers[i] = rand();
	}

	for (i = 0; i < count_of_strings - 1; ++i)
	{
		for (j = 0; j < count_of_strings - i - 1; ++j)
		{
			if (rand_numbers[j] > rand_numbers[j + 1])
			{
				temp = rand_numbers[j];
				rand_numbers[j] = rand_numbers[j + 1];
				rand_numbers[j + 1] = temp;

				temp_ptr = array_of_pointers_on_strings[j];
				array_of_pointers_on_strings[j] = array_of_pointers_on_strings[j + 1];
				array_of_pointers_on_strings[j + 1] = temp_ptr;
			}
		}
	}

	my_strcpy(*result, *array_of_pointers_on_strings);

	for (i = 1; i < count_of_strings; ++i)
	{
		my_strcat(*result, array_of_pointers_on_strings[i]);
	}

	free(rand_numbers);

	return 0;
}

int main(
	int argc,
	char *argv[]) 
{
	if (argc < 3) 
	{
		printf("Usage: %s <flag> <string> [<unsigned int> <strings> ...]\n", argv[0]);
		
		return EXIT_FAILURE;
	}

	char* flag = argv[1];
	char* str = argv[2];
	char* result = NULL;

	if (my_strlen(flag) != 2 || flag[0] != '-') 
	{
		printf("Invalid flag!\n");
		
		return EXIT_FAILURE;
	}

	switch (flag[1]) 
	{
	case 'l':
		printf("Length: %zu\n", my_strlen(str));
		
		break;

	case 'r':
		switch(reverse_str(str, &result)) 
		{
		case 0:
			printf("Reversed str: %s\n", result);
			
			break;

		case 1:
			printf("Memory allocation error!\n");
			
			break;

		default:
			printf("Unknown error in reverse_str func! =)\n");
			
			break;
		}
		
		break;

	case 'u':
		switch (to_uppercase_odd_positions(str, &result)) 
		{
		case 0:
			printf("Uppercased odd positions str: %s\n", result);
			
			break;

		case 1:
			printf("Memory allocation error!\n");
			
			break;

		default:
			printf("Unknown error in to_uppercase_odd_positions func! =))\n");
			
			break;
		}
		
		break;

	case 'n':
		switch (rearrange(str, &result)) 
		{
		case 0:
			printf("Rearranged str: %s\n", result);
			
			break;

		case 1:
			printf("Memory allocation error!\n");
			
			break;

		default:
			printf("Unknown error in rearrange func! =)))\n");
			
			break;
		}
		
		break;

	case 'c':
		if (argc < 4) 
		{
			printf("Usage: %s <flag> <string> <unsigned int> <strings> ...\n", argv[0]);
			
			return EXIT_FAILURE;
		}

		size_t seed_length = my_strlen(argv[3]);
		int i, j;

		for (i = 0; i < seed_length; i++) 
		{
			if (!isdigit(argv[3][i])) 
			{
				printf("Usage: %s <flag> <string> <unsigned int> <strings> ...\n", argv[0]);
				
				return EXIT_FAILURE;
			}
		}

		unsigned int seed = (unsigned int)atoi(argv[3]);

		char** array_of_pointers_on_strings = (char**)malloc((argc - 3) * sizeof(char*));

		if (array_of_pointers_on_strings == NULL) 
		{
			printf("Memory allocation error!\n");
			
			return EXIT_FAILURE;
		}

		for (i = 2, j = 0; i < argc; i++) 
		{
			if (i == 3) 
			{
				continue;
			}

			array_of_pointers_on_strings[j++] = argv[i];
		}

		size_t count_of_strings = argc - 3;

		switch (concatenate_strings_with_srand(array_of_pointers_on_strings, &result, seed, count_of_strings)) 
		{
		case 0:
			printf("Concateneded str: %s\n", result);

			break;

		case 1:
			printf("Invalid pointer!\n");

			break;

		case 2:
			printf("Memory allocation error!\n");

			break;

		default:
			printf("Unknown error in concatenate_strings_with_srand func! =))))\n");

			break;
		}

		free(array_of_pointers_on_strings);

		break;

	default:
		printf("Undefind behavior .___.\n");
		break;
	}

	free(result);

	return EXIT_SUCCESS;
}