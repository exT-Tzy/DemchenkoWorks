#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

enum status_codes 
{
    STATUS_SUCCESS = 0,
    STATUS_INVALID_ARGS = 1,
    STATUS_EMPTY_FILENAME = 2,
    STATUS_SAME_FILES = 3,
    STATUS_SOURCE_OPEN_ERROR = 4,
    STATUS_DEST_OPEN_ERROR = 5,
    STATUS_READ_ERROR = 6,
    STATUS_WRITE_ERROR = 7,
    STATUS_SOURCE_CLOSE_ERROR = 8,
    STATUS_DEST_CLOSE_ERROR = 9
};

int validate_arguments(
    int argc, 
    char *argv[]) 
{
    if (argc != 3) 
    {
        printf("Usage: %s <source_file> <destination_file>\n", argv[0]); 
        
        return STATUS_INVALID_ARGS;
    }

    if (strlen(argv[1]) == 0 || strlen(argv[2]) == 0) 
    {
        fprintf(stderr, "Error: File names cannot be empty\n");
        
        return STATUS_EMPTY_FILENAME;
    }

    if (strcmp(argv[1], argv[2]) == 0) // TODO: compare strings by abs paths =)
    {
        fprintf(stderr, "Error: Source and destination files cannot be the same\n");
        
        return STATUS_SAME_FILES;
    }

    return STATUS_SUCCESS;
}

int open_source_file(
    char const *filename, 
    FILE **file) 
{
    *file = fopen(filename, "rb");
    if (!file) 
    {
        fprintf(stderr, "Error: Cannot open source file '%s'\n", filename);
        
        return STATUS_SOURCE_OPEN_ERROR;
    }
    
    return STATUS_SUCCESS;
}

int open_dest_file(
    char const *filename, 
    FILE **file) 
{
    *file = fopen(filename, "wb");
    if (!file) 
    {
        fprintf(stderr, "Error: Cannot create destination file '%s'\n", filename);
        
        return STATUS_DEST_OPEN_ERROR;
    }
    
    return STATUS_SUCCESS;
}

int copy_file_data(
    FILE *source, 
    FILE *dest, 
    char const *source_name, 
    char const *dest_name) 
{
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    size_t bytes_written;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, source)) > 0) 
    {
        bytes_written = fwrite(buffer, 1, bytes_read, dest);
        
        if (bytes_written != bytes_read) 
        {
            fprintf(stderr, "Error: Cannot write to destination file '%s'\n", dest_name);
            
            return STATUS_WRITE_ERROR;
        }
    }

    if (ferror(source)) 
    {
        fprintf(stderr, "Error: Cannot read from source file '%s'\n", source_name);
        
        return STATUS_READ_ERROR;
    }

    return STATUS_SUCCESS;
}

int close_source_file(
    FILE *file) 
{
    if (file != NULL && fclose(file) != 0) 
    {
        fprintf(stderr, "Error: Cannot close source file\n");
        
        return STATUS_SOURCE_CLOSE_ERROR;
    }
   
    return STATUS_SUCCESS;
}

int close_dest_file(
    FILE *file) 
{
    if (file != NULL && fclose(file) != 0) 
    {
        fprintf(stderr, "Error: Cannot close destination file\n");
        
        return STATUS_DEST_CLOSE_ERROR;
    }
    
    return STATUS_SUCCESS;
}

int perform_file_copy(
    char const *source_name, 
    char const *dest_name) 
{
    FILE *source_file;
    FILE *dest_file;
    int status;
    int close_status_src;
    int close_status_dest;

    source_file = NULL;
    dest_file = NULL;

    status = open_source_file(source_name, &source_file);
    if (status != STATUS_SUCCESS) 
    {
        return status;
    }

    status = open_dest_file(dest_name, &dest_file);
    if (status != STATUS_SUCCESS) 
    {
        close_source_file(source_file);
        
        return status;
    }

    printf("Copying file '%s' to '%s'...\n", source_name, dest_name);

    status = copy_file_data(source_file, dest_file, source_name, dest_name);

    close_status_src = close_source_file(source_file);
    close_status_dest = close_dest_file(dest_file);

    if (status != STATUS_SUCCESS) 
    {
        return status;
    }

    if (close_status_src != STATUS_SUCCESS) 
    {
        return close_status_src;
    }

    if (close_status_dest != STATUS_SUCCESS) 
    {
        return close_status_dest;
    }

    printf("File copy completed successfully!\n");
   
    return STATUS_SUCCESS;
}

int main(
    int argc, 
    char *argv[]) 
{
    int validation_status;
    int copy_status;

    validation_status = validate_arguments(argc, argv);
    if (validation_status != STATUS_SUCCESS) 
    {
        return validation_status;
    }

    copy_status = perform_file_copy(argv[1], argv[2]);
    
    return copy_status;
}