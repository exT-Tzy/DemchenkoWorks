#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

#define RESULT_DECOMPOSITIONS_PARAMETER_IS_NULL 1
#define RESULT_DECOMPOSITIONS_COUNT_PARAMETER_IS_NULL 2
#define VALUE_IS_NOT_NATURAL 3
#define MEMORY_ALLOCATION_ERROR 4
#define CURRENT_PARAMETER_IS_NULL 5
#define CAPACITY_PARAMETER_IS_NULL 6

typedef enum
{
    decline_repeats = 0,
    allow_repeats = 1
} allowed_equal_sum_components_flag;

void free_decompositions(
    int** decompositions,
    size_t count)
{
    int i;

    for (i = 0; i < count; ++i)
    {
        free(decompositions[i]);
    }

    free(decompositions);
}

void decompose(
    int value,
    int start,
    allowed_equal_sum_components_flag allowed_equal_sum_components,
    int* current,
    int current_size,
    int*** result,
    size_t* count,
    size_t* capacity)
{
    int i, next_start;

    if (!current) return CURRENT_PARAMETER_IS_NULL;
    if (!result) return RESULT_DECOMPOSITIONS_PARAMETER_IS_NULL;
    if (!count) return RESULT_DECOMPOSITIONS_COUNT_PARAMETER_IS_NULL;
    if (!capacity) return CAPACITY_PARAMETER_IS_NULL;

    if (value == 0)
    {
        if (*count >= *capacity)
        {
            *capacity *= 2;

            if (_realloc((void**)result, *capacity * sizeof(int*))) return MEMORY_ALLOCATION_ERROR;
        }

        (*result)[*count] = malloc((current_size + 1) * sizeof(int));

        if (!(*result)[*count]) return EXIT_SUCCESS;

        (*result)[*count][0] = current_size;

        for (i = 0; i < current_size; ++i)
        {
            (*result)[*count][i + 1] = current[i];
        }

        (*count)++;

        return EXIT_SUCCESS;
    }

    for (i = start; i <= value; ++i)
    {
        current[current_size] = i;
        next_start = (allowed_equal_sum_components == decline_repeats) ? i + 1 : i;

        decompose(value - i, next_start, allowed_equal_sum_components, current, current_size + 1, result, count, capacity);
    }

    return EXIT_SUCCESS;
}

int sums_decomposition(
    int value,
    int*** result_decompositions,
    size_t* result_decompositions_count,
    allowed_equal_sum_components_flag allowed_equal_sum_components)
{
    size_t capacity;

    if (!result_decompositions) return RESULT_DECOMPOSITIONS_PARAMETER_IS_NULL;
    if (!result_decompositions_count) return RESULT_DECOMPOSITIONS_COUNT_PARAMETER_IS_NULL;
    if (value <= 0) return VALUE_IS_NOT_NATURAL;
    *result_decompositions = malloc(16 * sizeof(int*));
    if (!*result_decompositions) return MEMORY_ALLOCATION_ERROR;

    *result_decompositions_count = 0;
    capacity = 16;

    int* current = malloc(value * sizeof(int));
    if (!current)
    {
        free(*result_decompositions);
        return MEMORY_ALLOCATION_ERROR;
    }

    decompose(value, 1, allowed_equal_sum_components, current, 0, result_decompositions,
        result_decompositions_count, &capacity);

    free(current);
    return EXIT_SUCCESS;
}

void print_decompositions(
    int** decompositions,
    size_t count)
{
    int i, j;

    for (i = 0; i < count; ++i)
    {
        printf("(");

        for (j = 1; j <= decompositions[i][0]; ++j)
        {
            printf("%d%s", decompositions[i][j], (j == decompositions[i][0]) ? "" : ", ");
        }

        printf(")\n");
    }
}

int main()
{
    int** decompositions;
    size_t decompositions_count;

    int status, value = 20;

    printf("Sums decompositions of %d with decline repeats:\n", value);
    status = sums_decomposition(value, &decompositions, &decompositions_count, decline_repeats);

    if (status == 0)
    {
        print_decompositions(decompositions, decompositions_count);
        free_decompositions(decompositions, decompositions_count);
    }
    else
    {
        printf("Error: %d\n", status);
    }

    printf("\nSums decompositions of %d with allow repeats:\n", value);
    status = sums_decomposition(value, &decompositions, &decompositions_count, allow_repeats);


    if (status == 0)
    {
        print_decompositions(decompositions, decompositions_count);
        free_decompositions(decompositions, decompositions_count);
    }
    else
    {
        printf("Error: %d\n", status);
    }

    return EXIT_SUCCESS;
}
