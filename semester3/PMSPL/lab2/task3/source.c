#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

enum status_t
{
    OK = 0,
    INVALID_ARGUMENT,
    COULDNT_OPEN_THE_FILE,
    STRING_IS_EMPTY,
    MEM_ALLOC_ERR
};

int calculate_substrings_count_in_file(
    char const * file_path,
    char const * substr,
    size_t *substrings_count)
{
    FILE* fi;
    char ch;
    size_t substr_length, substr_index = 0, substr_count = 0;

    if (!file_path || !substr)
    {
        return INVALID_ARGUMENT;
    }

    substr_length = strlen(substr);
    if (substr_length == 0)
    {
        return STRING_IS_EMPTY;
    }

    printf("File %s:\n", file_path);

    fi = fopen(file_path, "r");
    if (!fi)
    {
        printf("    couldnt open\n");
        *substrings_count = 0;

        return OK;
    }

    while ((ch = (char)fgetc(fi)) != EOF)
    {
        if (ch == substr[substr_index])
        {
            if (substr_index == substr_length - 1)
            {
                ++substr_count;
                substr_index = 0;
            }
            else
            {
                ++substr_index;
            }
        }
        else
        {
            if (ch == substr[0])
            {
                substr_index = 1;
            }
            else
            {
                substr_index = 0;
            }
        }
    }

    printf("    %zu\n", substr_count);

    *substrings_count = substr_count;

    fclose(fi);

    return OK;
}

int free_array(
    char ***arr,
    size_t count_of_elements)
{
    int i;

    if (!arr)
    {
        return INVALID_ARGUMENT;
    }

    for (i = 0; i < count_of_elements; ++i)
    {
        free((*arr)[i]);
    }

    free(*arr);

    return OK;
}

void create_balanced_fork_tree(
    size_t n) 
{
    pid_t left_child, right_child;
    int left_status, right_status;
    size_t current_depth;

    if (n == 0) 
    {
        return;
    }

    left_child = fork();

    if (left_child < 0) 
    {
        exit(EXIT_FAILURE);
    }

    if (left_child == 0) 
    {
        create_balanced_fork_tree(n - 1);
       
        exit(EXIT_SUCCESS);
    }

    right_child = fork();

    if (right_child < 0) 
    {
        waitpid(left_child, &left_status, 0); 
        
        exit(EXIT_FAILURE);
    }

    if (right_child == 0) 
    {
        create_balanced_fork_tree(n - 1);
        
        exit(EXIT_SUCCESS);
    }

    waitpid(left_child, &left_status, 0);
    waitpid(right_child, &right_status, 0);
}

int main(
    int argc,
    char * argv[])
{
    int i;
    char filepath_buf[BUFSIZ], *filepath;
    FILE* paths_list_file;
    int status;
    size_t all_substrings_count = 0, substrings_in_file_count, filepath_length;
    pid_t pid;
    int pipe_fd[2];
    char** filepaths_arr, **for_realloc;
    size_t virtual_count_of_elements_in_arr = 0, size_of_arr = 8;
    
    if (argc != 3)
    {
        printf("Usage: %s <list_file_path> <substring_to_search>\n", argv[0]);

        return OK;
    }

    paths_list_file = fopen(argv[1], "r");
    if (!paths_list_file)
    {
        fprintf(stderr, "couldnt open %s\n", argv[1]);

        return OK;
    }

    filepaths_arr = (char**)malloc(sizeof(char*) * size_of_arr);
    if (!filepaths_arr)
    {
        fclose(paths_list_file);

        return MEM_ALLOC_ERR;
    }

    while (fgets(filepath_buf, BUFSIZ - 1, paths_list_file) != NULL)
    {   
        filepath_length = strlen(filepath_buf);
        if (filepath_buf[filepath_length - 1] == '\n')
        {
            filepath_buf[filepath_length - 1] = 0;
            --filepath_length;
        }

        if (filepath_length == 0)
        {
            continue;
        }

        if (virtual_count_of_elements_in_arr == size_of_arr)
        {
            for_realloc = (char**)realloc(filepaths_arr, sizeof(char*) * size_of_arr * 2);
            if (!for_realloc)
            {
                free_array(&filepaths_arr, virtual_count_of_elements_in_arr);
                fclose(paths_list_file);

                return MEM_ALLOC_ERR;
            }

            filepaths_arr = for_realloc;
            for_realloc = NULL;
            size_of_arr *= 2;
        }

        filepaths_arr[virtual_count_of_elements_in_arr] = (char*)malloc(sizeof(char) * (filepath_length + 1));
        if (!filepaths_arr[virtual_count_of_elements_in_arr])
        {
            free_array(&filepaths_arr, virtual_count_of_elements_in_arr);
            fclose(paths_list_file);

            return MEM_ALLOC_ERR;
        }

        strcpy(filepaths_arr[virtual_count_of_elements_in_arr], filepath_buf);
        ++virtual_count_of_elements_in_arr;
    }

    for (i = 0; i < virtual_count_of_elements_in_arr; ++i)
    {
        
        if (pipe(pipe_fd) == -1)
        {
            perror("couldnt create pipe\n");

            free_array(&filepaths_arr, virtual_count_of_elements_in_arr);
            fclose(paths_list_file);

            return OK;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork error\n");

            return OK;
        }
        else if (pid == 0)
        {
            close(pipe_fd[0]);

            substrings_in_file_count = 0;
            calculate_substrings_count_in_file(filepaths_arr[i], argv[2], &substrings_in_file_count);

            write(pipe_fd[1], &substrings_in_file_count, sizeof(substrings_in_file_count));
            close(pipe_fd[1]);

            exit(OK);
        }
        else
        {
            close(pipe_fd[1]);

            read(pipe_fd[0], &substrings_in_file_count, sizeof(substrings_in_file_count));
            close(pipe_fd[0]);

            waitpid(pid, &status, 0);

            all_substrings_count += substrings_in_file_count;
        }
    }

    free_array(&filepaths_arr, virtual_count_of_elements_in_arr);
    fclose(paths_list_file);

    printf("All substrings count: %zu\n", all_substrings_count);
    if (all_substrings_count == 0)
    {
        printf("BEEEEE\n");
        create_balanced_fork_tree(strlen(argv[2]));
    }

    return OK;
}