#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

enum 
{
    SUCCESS = 0,
    ERROR,
    INVALID_ARGS,
    MEMORY_ERROR,
    FILE_EXISTS,
    FILE_ERROR
};

typedef struct file_node 
{
    ino_t _inode;
    dev_t _device;
    struct file_node* _next;
} file_node;

int add_file_to_list(
    file_node** head, 
    ino_t inode, 
    dev_t device) 
{
    file_node* new_node = NULL;
    file_node* current = NULL;

    current = *head;
    while (current != NULL) 
    {
        if (current->_inode == inode && current->_device == device) 
        {
            return FILE_EXISTS; 
        }
        
        current = current->_next;
    }

    new_node = (file_node*)malloc(sizeof(file_node));
    if (new_node == NULL) 
    {
        return MEMORY_ERROR;
    }
    
    new_node->_inode = inode;
    new_node->_device = device;
    new_node->_next = *head;
    *head = new_node;
    
    return SUCCESS; 
}

void free_file_list(
    file_node* head) 
{
    file_node* current = head;
    file_node* next = NULL;
    
    while (current != NULL) 
    {
        next = current->_next;
        
        free(current);
       
        current = next;
    }
}

char const* get_file_extension(
    char const* filename) 
{
    char const* dot = NULL;
    
    dot = strrchr(filename, '.');
    if (dot == NULL || dot == filename) 
    {
        return "";
    }
    
    return dot + 1;
}

int print_file_info(
    char const* filepath, 
    file_node** printed_files) 
{
    struct stat file_stat;
    int add_result = 0;
    char const* filename = NULL;
    char const* extension = NULL;
    
    if (stat(filepath, &file_stat) != 0) 
    {
        return FILE_ERROR;
    }

    if (!S_ISREG(file_stat.st_mode)) 
    {
        return SUCCESS;
    }

    add_result = add_file_to_list(printed_files, file_stat.st_ino, file_stat.st_dev);
    if (add_result == FILE_EXISTS) 
    {
        return SUCCESS;
    }
    if (add_result == MEMORY_ERROR) 
    {
        return MEMORY_ERROR;
    }

    filename = strrchr(filepath, '/');
    if (filename == NULL) 
    {
        filename = filepath;
    } 
    else 
    {
        filename++;
    }
    
    extension = get_file_extension(filename);

    printf("%-50s %-20s %lu\n", filename, extension, (unsigned long)file_stat.st_ino);
    
    return SUCCESS;
}

int traverse_directory(
    char const* dir_path, 
    int current_depth, 
    int recmin, 
    int recmax, 
    file_node** printed_files) 
{
    DIR* dir = NULL;
    struct dirent* entry = NULL;
    char full_path[4096];
    struct stat path_stat;
    int result = 0;
    
    if (current_depth > recmax) 
    {
        return SUCCESS;
    }
    
    dir = opendir(dir_path);
    if (dir == NULL) 
    {
        return ERROR;
    }
    
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
        {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
        
        if (stat(full_path, &path_stat) != 0) 
        {
            continue;
        }
        
        if (S_ISREG(path_stat.st_mode)) 
        {
            if (current_depth >= recmin) 
            {
                result = print_file_info(full_path, printed_files);
                if (result != SUCCESS) 
                {
                    closedir(dir);
                    
                    return result;
                }
            }
        } 
        else if (S_ISDIR(path_stat.st_mode)) 
        {
            result = traverse_directory(full_path, current_depth + 1, recmin, recmax, printed_files);
            if (result != SUCCESS) 
            {
                closedir(dir);
                
                return result;
            }
        }
    }
    
    closedir(dir);
   
    return SUCCESS;
}

int validate_arguments(
    int argc, 
    char* argv[], 
    int* recmin, 
    int* recmax) 
{
    int i = 0;
    
    if (argc < 4) 
    {
        printf("Usage: %s <recmin> <recmax> <directory1> [<directory2> ...]\n", argv[0]);
        
        return INVALID_ARGS;
    }
    
    *recmin = atoi(argv[1]);
    *recmax = atoi(argv[2]);
    
    if (*recmin < 0 || *recmax < 0 || *recmin > *recmax) 
    {
        printf("recmin and recmax must be non-negative integers with recmin <= recmax\n");
        
        return INVALID_ARGS;
    }

    for (i = 3; i < argc; i++) 
    {
        struct stat path_stat;
        if (stat(argv[i], &path_stat) != 0) 
        {
            printf("Cannot access path '%s'\n", argv[i]);
            
            return INVALID_ARGS;
        }
        if (!S_ISDIR(path_stat.st_mode)) 
        {
            printf("'%s' is not a directory\n", argv[i]);
          
            return INVALID_ARGS;
        }
    }
    
    return SUCCESS;
}

int main(
    int argc, 
    char* argv[]) 
{
    int recmin = 0;
    int recmax = 0;
    int i = 0;
    int result = 0;
    file_node* printed_files = NULL;

    result = validate_arguments(argc, argv, &recmin, &recmax);
    if (result != SUCCESS) 
    {
        return INVALID_ARGS;
    }
    
    printf("%-50s %-20s %s\n", "Filename", "Extension", "Inode");
    printf("%-50s %-20s %s\n", "--------", "---------", "-----");

    for (i = 3; i < argc; i++) 
    {
        printf("\nDirectory: %s\n", argv[i]);
        
        result = traverse_directory(argv[i], 0, recmin, recmax, &printed_files);
        if (result != SUCCESS) 
        {
            printf("Failed to traverse directory '%s'\n", argv[i]);
            
            free_file_list(printed_files);
            
            return INVALID_ARGS;
        }
    }

    free_file_list(printed_files);
    
    return SUCCESS;
}