#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum {
    STATUS_SUCCESS = 0,
    STATUS_INVALID_ARGS,
    STATUS_FILE_ERROR,
    STATUS_UNKNOWN_OPERATION,
    STATUS_MEMORY_ERROR,
    STATUS_READ_ERROR
} status_code;


status_code process_xor8(
    FILE *file, 
    uint8_t *result) 
{
    uint8_t byte;
    uint8_t xor_result;
    size_t bytes_read;
    
    xor_result = 0;
    
    while ((bytes_read = fread(&byte, sizeof(uint8_t), 1, file)) == 1) 
    {
        xor_result ^= byte;
    }
    
    if (ferror(file)) 
    {
        return STATUS_READ_ERROR;
    }
    
    *result = xor_result;
    
    return STATUS_SUCCESS;
}

status_code is_prime(
    uint8_t num, 
    int *result) 
{
    int i;
    
    *result = 0;
    
    if (num < 2) 
    {
        return STATUS_SUCCESS;
    }
    
    if (num == 2) 
    {
        *result = 1;
        
        return STATUS_SUCCESS;
    }
    
    if (num % 2 == 0) 
    {
        return STATUS_SUCCESS;
    }
    
    for (i = 3; i * i <= num; i += 2) 
    {
        if (num % i == 0) 
        {
            return STATUS_SUCCESS;
        }
    }
    
    *result = 1;
    
    return STATUS_SUCCESS;
}

status_code has_prime_byte(
    uint32_t value, 
    int *result) 
{
    uint8_t bytes[4];
    int i;
    int is_prime_result;
    status_code status;
    
    *result = 0;

    bytes[0] = (uint8_t)(value & 0xFF);
    bytes[1] = (uint8_t)((value >> 8) & 0xFF);
    bytes[2] = (uint8_t)((value >> 16) & 0xFF);
    bytes[3] = (uint8_t)((value >> 24) & 0xFF);
    
    for (i = 0; i < 4; i++) 
    {
        status = is_prime(bytes[i], &is_prime_result);
        if (status != STATUS_SUCCESS) 
        {
            return status;
        }
        
        if (is_prime_result) 
        {
            *result = 1;
           
            return STATUS_SUCCESS;
        }
    }
    
    return STATUS_SUCCESS;
}

status_code process_xorodd(
    FILE *file, 
    uint32_t *result) 
{
    uint32_t value;
    uint32_t xor_result;
    size_t bytes_read;
    int has_prime;
    status_code status;
    
    xor_result = 0;
    
    while ((bytes_read = fread(&value, sizeof(uint32_t), 1, file)) == 1) 
    {
        status = has_prime_byte(value, &has_prime);
        if (status != STATUS_SUCCESS) 
        {
            return status;
        }
        
        if (has_prime) 
        {
            xor_result ^= value;
        }
    }
    
    if (ferror(file)) 
    {
        return STATUS_READ_ERROR;
    }
    
    *result = xor_result;
    
    return STATUS_SUCCESS;
}

status_code parse_hex_mask(
    char const *hex_str, 
    uint32_t *mask) 
{
    char *endptr;
    unsigned long parsed_value;
    
    if (hex_str == NULL || mask == NULL) 
    {
        return STATUS_INVALID_ARGS;
    }
    
    parsed_value = strtoul(hex_str, &endptr, 16);
    
    if (*endptr != '\0' || parsed_value > UINT32_MAX) 
    {
        return STATUS_INVALID_ARGS;
    }
    
    *mask = (uint32_t)parsed_value;
    
    return STATUS_SUCCESS;
}

status_code process_mask(
    FILE *file, 
    uint32_t mask, 
    int *count) 
{
    uint32_t value;
    int match_count;
    size_t bytes_read;
    
    match_count = 0;
    
    while ((bytes_read = fread(&value, sizeof(uint32_t), 1, file)) == 1) 
    {
        if ((value & mask) == mask) 
        {
            match_count++;
        }
    }
    
    if (ferror(file)) 
    {
        return STATUS_READ_ERROR;
    }
    
    *count = match_count;
    
    return STATUS_SUCCESS;
}

void print_usage(const char *program_name) {
    printf("Usage: %s <input_file> <operation> [mask]\n", program_name);
    printf("Operations:\n");
    printf("  xor8    - XOR all bytes in the file\n");
    printf("  xorodd  - XOR 4-byte sequences containing at least one prime byte\n");
    printf("  mask <hex_mask> - Count 4-byte integers matching the hexadecimal mask\n");
}

void print_error(status_code status) 
{
    switch (status) 
    {
        case STATUS_INVALID_ARGS:
            
            printf("Error: Invalid arguments\n");
            
            break;
        
        case STATUS_FILE_ERROR:
            
            printf("Error: Cannot open file\n");
            
            break;
        
        case STATUS_UNKNOWN_OPERATION:
            
            printf("Error: Unknown operation\n");
            
            break;
        
        case STATUS_MEMORY_ERROR:
            
            printf("Error: Memory allocation failed\n");
            
            break;
        
        case STATUS_READ_ERROR:
            
            printf("Error: File read error\n");
           
            break;
        
        default:
            
            printf("Error: Unknown error\n");
            
            break;
    }
}

int main(int argc, char *argv[]) 
{
    FILE *input_file;
    status_code status;
    uint8_t xor8_result;
    uint32_t xorodd_result;
    uint32_t mask;
    int mask_count;

    if (argc < 3) {
        print_usage(argv[0]);
        
        return STATUS_INVALID_ARGS;
    }

    input_file = fopen(argv[1], "rb");
    if (input_file == NULL) 
    {
        print_error(STATUS_FILE_ERROR);
        
        return STATUS_FILE_ERROR;
    }

    if (strcmp(argv[2], "xor8") == 0) 
    {
        status = process_xor8(input_file, &xor8_result);
        if (status == STATUS_SUCCESS) 
        {
            printf("XOR8 result: 0x%02X (%u)\n", xor8_result, xor8_result);
        }
    }
    else if (strcmp(argv[2], "xorodd") == 0) 
    {
        status = process_xorodd(input_file, &xorodd_result);
        if (status == STATUS_SUCCESS) 
        {
            printf("XORODD result: 0x%08X (%u)\n", xorodd_result, xorodd_result);
        }
    }
    else if (strcmp(argv[2], "mask") == 0) 
    {
        if (argc < 4) 
        {
            printf("Error: Mask operation requires hexadecimal mask parameter\n");
            fclose(input_file);
            
            return STATUS_INVALID_ARGS;
        }
        
        status = parse_hex_mask(argv[3], &mask);
        if (status != STATUS_SUCCESS) 
        {
            printf("Error: Invalid hexadecimal mask format\n");
            fclose(input_file);
            
            return STATUS_INVALID_ARGS;
        }
        
        status = process_mask(input_file, mask, &mask_count);
        if (status == STATUS_SUCCESS) 
        {
            printf("MASK result: %d matches for mask 0x%08X\n", mask_count, mask);
        }
    }
    else 
    {
        status = STATUS_UNKNOWN_OPERATION;
    }
    
    fclose(input_file);
    
    if (status != STATUS_SUCCESS) 
    {
        print_error(status);
    }
    
    return status;
}