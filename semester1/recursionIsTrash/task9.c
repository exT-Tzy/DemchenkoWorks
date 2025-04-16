#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "memory.h"

#define NUMBER_IS_NOT_IN_BASE 1
#define INVALID_BASE 2
#define NULL_PTR 3
#define MEMORY_ALLOCATION_ERROR 4

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

    if (!*number)
    {
        number[0] = '0';
        number[1] = 0;
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

    *result_placement = temp;

    return EXIT_SUCCESS;
}

int string_compare(
    char const* a,
    char const* b)
{
    int len_a;
    int len_b;

    if (!a || !b) return 0;


    len_a = strlen(a);
    len_b = strlen(b);

    if (len_a > len_b) return 1;
    if (len_a < len_b) return 0;

    return (strcmp(a, b) > 0);
}

int generate_Fibonacci_sequence_recursive(
    const char* number,
    size_t const number_length,
    char*** fib,
    size_t* size,
    size_t* capacity)
{
    char* next;
    size_t next_length;
    int status;

    if (!number || !fib || !size || !capacity) return NULL_PTR;

    status = add_two_numbers((*fib)[*size - 1], (*fib)[*size - 2], 10, &next);
    if (status) return status;

    next_length = strlen(next);

    if (next_length > number_length || (next_length == number_length && string_compare(next, number) == 1))
    {
        free(next);
        return EXIT_SUCCESS;
    }

    ++(*size);

    if (*size == *capacity)
    {
        *capacity *= 2;
        if (_realloc((void**)fib, *capacity * sizeof(char*))) return MEMORY_ALLOCATION_ERROR;
    }

    (*fib)[*size - 1] = next;

    status = generate_Fibonacci_sequence_recursive(number, number_length, fib, size, capacity);
    if (status) return status;

    return EXIT_SUCCESS;
}

int Fibonacci_sequence_destroyer(
    char*** sequence,
    size_t count_of_elements)
{
    size_t i = 0;

    if (!sequence || !*sequence) return NULL_PTR;

    for (i = 0; i < count_of_elements; ++i)
    {

        ((*sequence)[i]);
    }

    free(*sequence);
    return EXIT_SUCCESS;
}

int main()
{
    char* number = "75213967518927638469712679352816357812389675213679856782315682138756231895623178568723169567812367513249678567823458679432985674327698594678235967834298756349678578964123652347689596783429567843297826569478325968743268754367856897432956782346785436927859678345678432786734267895765489763285679454396785968724385347659834726567843268574328765946783257864326785432978654938672423785437968597843";

    char** fib;
    size_t count_of_elements = 2, capacity = 8, i, num_length;
    int status;

    fib = (char**)malloc(capacity * sizeof(char*));
    if (!fib) return MEMORY_ALLOCATION_ERROR;

    status = delete_top_zeros(number);
    if (status) return status;

    num_length = strlen(number);

    fib[0] = (char*)malloc(sizeof(char) * 2);
    if (!fib[0])
    {
        free(fib);
        return MEMORY_ALLOCATION_ERROR;
    }
    fib[1] = (char*)malloc(sizeof(char) * 2);
    if (!fib[1])
    {
        free(fib[0]);
        free(fib);
        return MEMORY_ALLOCATION_ERROR;
    }

    strcpy(fib[0], "1");
    strcpy(fib[1], "2");

    status = generate_Fibonacci_sequence_recursive(number, num_length, &fib, &count_of_elements, &capacity);
    if (status) return status;

    for (i = 0; i < count_of_elements; ++i)
    {
        printf("%s\n", fib[i]);
    }

    status = Fibonacci_sequence_destroyer(&fib, count_of_elements);
    if (status) return status;

    return EXIT_SUCCESS;
}
