#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "memory.h"

#define TO_FIND_PARAMETER_IS_NULL 1
#define RESULTS_PARAMETER_IS_NULL 2
#define POSITIONS_PARAMETER_IS_NULL 3
#define RESULTS_COUNT_PARAMETER_IS_NULL 4
#define MEMORY_ALLOCATION_ERROR 5

typedef enum
{
	case_insentive = 0,
	case_sentive = 1
} case_sensitive_flag;

void SVOboda(
	char** results,
	int** positions,
	size_t results_and_positions_capacity)
{
	int i;

	for (i = 0; i < results_and_positions_capacity; ++i)
	{
		free(positions[i]);
	}

	free(results);
	free(positions);
}

int to_find_substring(
	const char* str,
	const char* substr,
	case_sensitive_flag case_sensitive)
{
	int i = 0;

	while ((substr[i]) && (str[i]) && ((case_sensitive) ? substr[i] == str[i] : toupper(substr[i]) == toupper(str[i])))
	{
		++i;
	}

	return substr[i] == 0;
}


int substr(
	char* to_find,
	case_sensitive_flag case_sensitive,
	char*** results,
	int*** positions,
	size_t* results_count,
	...)
{
	va_list args;
	char* to_find_prt = to_find, *str;
	size_t results_and_positions_capacity = 16, positions_index, inside_positions_array_capacity;
	int i, is_substr_found;

	if (!to_find) return TO_FIND_PARAMETER_IS_NULL;
	if (!results) return RESULTS_PARAMETER_IS_NULL;
	if (!positions) return POSITIONS_PARAMETER_IS_NULL;
	if (!results_count) return RESULTS_COUNT_PARAMETER_IS_NULL;

	*results = (char**)malloc(results_and_positions_capacity * sizeof(char*));

	if (!*results) return MEMORY_ALLOCATION_ERROR;

	*positions = (int**)malloc(results_and_positions_capacity * sizeof(int*));

	if (!*positions)
	{
		free(*results);

		return MEMORY_ALLOCATION_ERROR;
	}

	va_start(args, results_count);

	*results_count = 0;

	while (1)
	{
		if ((str = va_arg(args, char*)) == NULL) break;

		i = 0;
		is_substr_found = 0;
		positions_index = 1;
		inside_positions_array_capacity = 16;

		while (str[i])
		{
			if (to_find_substring(str + i, to_find, case_sensitive)) 
			{
				if (!is_substr_found)
				{
					if (*results_count == results_and_positions_capacity)
					{
						results_and_positions_capacity *= 2;

						if (_realloc((void**)results, results_and_positions_capacity * sizeof(char *)))
						{
							SVOboda(*results, *positions, results_and_positions_capacity);
							va_end(args);

							return MEMORY_ALLOCATION_ERROR;
						}
						if (_realloc((void**)positions, results_and_positions_capacity * sizeof(int*)))
						{
							SVOboda(*results, *positions, results_and_positions_capacity);
							va_end(args);

							return MEMORY_ALLOCATION_ERROR;
						}
					}

					(*positions)[*results_count] = (int*)malloc(inside_positions_array_capacity * sizeof(int));

					if ((*positions)[*results_count] == NULL)
					{
						SVOboda(*results, *positions, results_and_positions_capacity);
						va_end(args);

						return MEMORY_ALLOCATION_ERROR;
					}

					(*positions)[*results_count][0] = 1;
					(*positions)[*results_count][positions_index++] = i;

					(*results)[*results_count] = str;
					*results_count += 1;
					is_substr_found = 1;
				}
				else
				{
					if (positions_index == inside_positions_array_capacity)
					{
						inside_positions_array_capacity *= 2;

						if (_realloc((void**)&(*positions)[*results_count - 1], inside_positions_array_capacity * sizeof(int)))
						{
							SVOboda(*results, *positions, results_and_positions_capacity);
							va_end(args);

							return MEMORY_ALLOCATION_ERROR;
						}
					}

					(*positions)[*results_count - 1][positions_index++] = i;
					(*positions)[*results_count - 1][0] += 1;
				}
			}

			++i;
		}
	}

	va_end(args);

	return EXIT_SUCCESS;
}

int main()
{
	char** results;
	int** positions;
	size_t results_count;
	int i, j;

	substr("SD", case_sentive, &results, &positions, &results_count, "asdasdaDsasdasdasdasdasdas", NULL);

	for (i = 0; i < results_count; ++i)
	{
		printf("%d str (%p): \n\t%d substrings on positions: ", i, results[i], positions[i][0]);

		for (j = 1; j <= positions[i][0]; ++j)
		{
			printf("%d ", positions[i][j]);
		}

		printf("\n");
	}

	SVOboda(results, positions, results_count);

	return EXIT_SUCCESS;
}