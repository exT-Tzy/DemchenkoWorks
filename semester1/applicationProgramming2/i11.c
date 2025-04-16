#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

#define INITIAL_PARAMETER_IS_NULL 1
#define DETECTOR_PARAMETER_IS_NULL 2
#define LEXEMS_PARAMETER_IS_NULL 3
#define LEXEMS_COUNT_PARAMETER_IS_NULL 4
#define MEMORY_ALLOCATION_ERROR 5

typedef enum
{
	decline_empty_lexems = 0,
	accept_empty_lexems = 1
} fate_of_empty_lexems_flag;

int detector_example(int c)
{
	return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9');
}

void Dzhango_osvobozhdenniy(
	char** lexems,
	size_t capacity)
{
	int i;

	for (i = 0; i < capacity; ++i)
	{
		free(lexems[i]);
	}

	free(lexems);
}

int tokenize(
	char* initial,
	int (*detector)(int),
	fate_of_empty_lexems_flag fate_of_empty_lexems,
	char*** lexems,
	size_t* lexems_count)
{
	size_t lexem_capacity, array_of_lexems_capacity = 8;
	char *current_initial_index = initial;
	int status, previous_character_is_bad = 0, lexem_index = 0, is_lexem_allocated = 0, character_index = 0;

	if (!initial) return INITIAL_PARAMETER_IS_NULL;
	if (!detector) return DETECTOR_PARAMETER_IS_NULL;
	if (!lexems) return LEXEMS_PARAMETER_IS_NULL;
	if (!lexems_count) return LEXEMS_COUNT_PARAMETER_IS_NULL;

	*lexems_count = 0;

	*lexems = (char**)malloc(array_of_lexems_capacity * sizeof(char*));

	if (!*lexems) return MEMORY_ALLOCATION_ERROR;

	while (1)
	{
		if (detector(*current_initial_index))
		{
			if (lexem_index == array_of_lexems_capacity)
			{
				array_of_lexems_capacity *= 2;
				status = _realloc((void**)lexems, array_of_lexems_capacity * sizeof(char *));

				if (status)
				{
					Dzhango_osvobozhdenniy(*lexems, array_of_lexems_capacity);

					return MEMORY_ALLOCATION_ERROR;
				}
			}
			if (!is_lexem_allocated)
			{
				lexem_capacity = 16;
				character_index = 0;
				(*lexems)[lexem_index] = (char*)malloc(lexem_capacity * sizeof(char));

				if (!(*lexems)[lexem_index])
				{
					Dzhango_osvobozhdenniy(*lexems, array_of_lexems_capacity);

					return MEMORY_ALLOCATION_ERROR;
				}

				is_lexem_allocated = 1;
			}
			if (character_index == lexem_capacity - 1)
			{
				lexem_capacity *= 2;
				status = _realloc((void**)&(*lexems)[lexem_index], lexem_capacity * sizeof(char));

				if (status)
				{
					Dzhango_osvobozhdenniy(*lexems, array_of_lexems_capacity);

					return MEMORY_ALLOCATION_ERROR;
				}
			}

			previous_character_is_bad = 0;
			(*lexems)[lexem_index][character_index++] = *current_initial_index;
		}
		else if (!detector(*current_initial_index))
		{
			if (previous_character_is_bad)
			{
				if (fate_of_empty_lexems == accept_empty_lexems)
				{
					if (lexem_index == array_of_lexems_capacity)
					{
						array_of_lexems_capacity *= 2;
						status = _realloc((void**)lexems, array_of_lexems_capacity * sizeof(char *));

						if (status)
						{
							Dzhango_osvobozhdenniy(*lexems, array_of_lexems_capacity);

							return MEMORY_ALLOCATION_ERROR;
						}
					}

					(*lexems)[lexem_index] = (char*)malloc(sizeof(char));

					if (!(*lexems)[lexem_index])
					{
						Dzhango_osvobozhdenniy(*lexems, array_of_lexems_capacity);

						return MEMORY_ALLOCATION_ERROR;
					}

					(*lexems)[lexem_index][0] = 0;
					++lexem_index;
					*lexems_count += 1;
				}
			}
			else
			{
				(*lexems)[lexem_index][character_index] = 0;
				character_index = 0;
				++lexem_index;
				*lexems_count += 1;
				is_lexem_allocated = 0;
				previous_character_is_bad = 1;
			}
			if (*current_initial_index == 0) break;
		}

		++current_initial_index;
	}

	return EXIT_SUCCESS;
}

int main()
{
	char** lexems;
	size_t lexems_count;
	int i;

	tokenize("SDFADS ADSFDAS ASD   SADF SAD DSA   ASDF", detector_example, accept_empty_lexems, &lexems, &lexems_count);

	for (i = 0; i < lexems_count; ++i)
	{
		printf("(%d) %s\n", i + 1, lexems[i]);
	}

	Dzhango_osvobozhdenniy(lexems, lexems_count);

	return EXIT_SUCCESS;
}
