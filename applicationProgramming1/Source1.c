#include <stdio.h>
#include <ctype.h>

int gorner(
    char const* str,
    int base,
    int* result_placement)
{
    int result = 0;
    int is_negative = 0;

    if (str == NULL || !*str)
    {
        return 1;
    }

    if (*str == '-')
    {
        ++str;
        is_negative = 1;

        if (!*str)
        {
            return 1;
        }
    }

    if (base < 2 || base > 36)
    {
        return 2;
    }

    if (result_placement == NULL)
    {
        return 3;
    }

    while (*str)
    {
        if (!((isdigit(*str) && *str - '0' < base) ||
            (isalpha(*str) && tolower(*str) - 'a' < base - 10)))
        {
            return 1;
        }

        result = result * base + (isdigit(*str)
            ? *str - '0'
            : tolower(*str) - 'a' + 10);

        ++str;
    }

    if (is_negative)
    {
        result = -result;
    }

    *result_placement = result;

    return 0;
}

int main() {
    char str[10] = "SDASDOK";
    int base = 36;
    int result;
    
    switch (gorner(str, base, &result))
    {
    case 0:
        printf("%s(%d) == %d(10)", str, base, result);
        break;
    case 1:
        printf("Invalid input string value!");
        break;
    case 2:
        printf("Invalid base value!");
        break;
    case 3:
        printf("Invalid result placement value!");
        break;
    default:
        printf("Undefined behavior o__O");
        break;
    }

	return 0;
}