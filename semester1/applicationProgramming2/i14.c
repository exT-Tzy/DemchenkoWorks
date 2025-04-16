#include <stdio.h> 
#include <stdlib.h> 

#define SEQUENCE_PARAMETER_IS_NULL 1 
#define SUBSEQUENCE_START_INDEX_STORAGE_PARAMETER_IS_NULL 2 
#define SUBSEQUENCE_LENGTH_STORAGE_PARAMETER_IS_NULL 3 
#define COMPARER_PARAMETER_IS_NULL 4 
#define INVALID_SEQUENCE_LENGTH 5

#define SEQUENCE_LENGTH 10 

typedef int tvalue;

typedef enum
{
    not_strict_comparison = 0,
    strict_comparison = 1
} comparison_strictness;

int int_comparer(
    tvalue const *a,
    tvalue const *b)
{
    return (*a - *b);
}

int find_the_longest_sawtooth_subsequence(
    tvalue const* sequence,
    size_t* subsequence_start_index_storage,
    size_t* subsequence_length_storage,
    int (*comparer)(tvalue const*, tvalue const*),
    comparison_strictness is_comparison_is_strict)
{
    int is_subsequence_started = 0, is_sawtooth_element_condition = 0;
    size_t temp_length = 0, start_index, i;
    tvalue first, second;

    if (!sequence) return SEQUENCE_PARAMETER_IS_NULL;
    if (!subsequence_start_index_storage) return SUBSEQUENCE_START_INDEX_STORAGE_PARAMETER_IS_NULL;
    if (!subsequence_length_storage) return SUBSEQUENCE_LENGTH_STORAGE_PARAMETER_IS_NULL;
    if (!comparer) return COMPARER_PARAMETER_IS_NULL;
    if (SEQUENCE_LENGTH < 0) return INVALID_SEQUENCE_LENGTH;

    *subsequence_start_index_storage = -1;
    *subsequence_length_storage = 0;

    for (i = 0; i < SEQUENCE_LENGTH; ++i)
    {
        if (!is_subsequence_started)
        {
            start_index = i;
        }

        if (i == 0)
        {
            second = comparer(sequence + i, sequence + i + 1);
            first = second;
        }
        else if (i == SEQUENCE_LENGTH - 1)
        {
            first = comparer(sequence + i, sequence + i - 1);
            second = first;
        }
        else
        {
            first = comparer(sequence + i, sequence + i - 1);
            second = comparer(sequence + i, sequence + i + 1);
        }

        if (is_comparison_is_strict)
        {
            is_sawtooth_element_condition = ((first < 0 && second < 0) || (first > 0 && second > 0));
        }
        else
        {
            is_sawtooth_element_condition = ((first <= 0 && second <= 0) || (first >= 0 && second >= 0));
        }

        if (is_sawtooth_element_condition)
        {
            is_subsequence_started = 1;
            ++temp_length;
        }
        else
        {
            is_subsequence_started = 0;

            if (temp_length > *subsequence_length_storage)
            {
                *subsequence_length_storage = temp_length;
                *subsequence_start_index_storage = start_index;
                temp_length = 0;
            }
        }
    }

    if (temp_length > *subsequence_length_storage)
    {
        *subsequence_length_storage = temp_length;
        *subsequence_start_index_storage = start_index;
    }

    return EXIT_SUCCESS;
}

int main()
{
    size_t length, index;
    tvalue sequence[SEQUENCE_LENGTH] = { 1, 2, 1, 2, 2, 3, 2, 3, 2, 3};

    find_the_longest_sawtooth_subsequence(sequence, &index, &length, int_comparer, strict_comparison);

    printf("%d %d", length, index);

    return EXIT_SUCCESS;
}