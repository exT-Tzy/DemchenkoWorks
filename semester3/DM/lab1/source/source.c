#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum status_codes
{
    SUCCESS = 0,
    ERROR_MEMORY,
    ERROR_INVALID_NAME,
    ERROR_SET_NOT_FOUND,
    ERROR_SET_EXISTS,
    ERROR_INVALID_COMMAND,
    ERROR_ELEMENT_NOT_FOUND,
    ERROR_EMPTY_SET
};

typedef struct node_t
{
    char data;
    struct node_t* next;
} node_t;

typedef struct set_list
{
    char name;
    node_t* elements;
    struct set_list* next;
} set_list;

int create_node(
    node_t** node, 
    char data)
{
    *node = (node_t*)malloc(sizeof(node_t));
   
    if (*node == NULL)
    {
        return ERROR_MEMORY;
    }
    
    (*node)->data = data;
    (*node)->next = NULL;
    
    return SUCCESS;
}

int find_set(
    set_list* sets, 
    char name, 
    set_list** result)
{
    set_list* current = sets;

    while (current != NULL)
    {
        if (current->name == name)
        {
            *result = current;
           
            return SUCCESS;
        }
       
        current = current->next;
    }
    
    return ERROR_SET_NOT_FOUND;
}

int add_new_set(
    set_list** sets,
    char name)
{
    set_list* new_set = NULL;
    set_list* current = NULL;

    if (!isupper(name))
    {
        return ERROR_INVALID_NAME;
    }

    if (find_set(*sets, name, &current) == SUCCESS)
    {
        return ERROR_SET_EXISTS;
    }

    new_set = (set_list*)malloc(sizeof(set_list));
    
    if (new_set == NULL)
    {
        return ERROR_MEMORY;
    }

    new_set->name = name;
    new_set->elements = NULL;
    new_set->next = *sets;
    *sets = new_set;

    return SUCCESS;
}

int delete_set(
    set_list** sets,
    char name)
{
    set_list* current = *sets;
    set_list* prev = NULL;
    node_t* elem = NULL;
    node_t* temp = NULL;

    while (current != NULL)
    {
        if (current->name == name)
        {
            elem = current->elements;
           
            while (elem != NULL)
            {
                temp = elem;
                elem = elem->next;
                
                free(temp);
            }

            if (prev == NULL)
            {
                *sets = current->next;
            }
            else
            {
                prev->next = current->next;
            }
           
            free(current);
            
            return SUCCESS;
        }
        
        prev = current;
        current = current->next;
    }
   
    return ERROR_SET_NOT_FOUND;
}

int add_element(
    set_list* sets,
    char set_name, 
    char element)
{
    set_list* set = NULL;
    node_t* new_node = NULL;
    node_t* current = NULL;
    node_t* prev = NULL;
    int status = 0;

    status = find_set(sets, set_name, &set);
   
    if (status != SUCCESS)
    {
        return status;
    }

    current = set->elements;
   
    while (current != NULL && current->data < element)
    {
        prev = current;
        current = current->next;
    }

    if (current != NULL && current->data == element)
    {
        return SUCCESS;
    }

    status = create_node(&new_node, element);
   
    if (status != SUCCESS)
    {
        return status;
    }

    new_node->next = current;
   
    if (prev == NULL)
    {
        set->elements = new_node;
    }
    else
    {
        prev->next = new_node;
    }

    return SUCCESS;
}

int remove_element(
    set_list* sets,
    char set_name,
    char element)
{
    set_list* set = NULL;
    node_t* current = NULL;
    node_t* prev = NULL;
    int status = 0;

    status = find_set(sets, set_name, &set);
   
    if (status != SUCCESS)
    {
        return status;
    }

    current = set->elements;
   
    while (current != NULL && current->data != element)
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL)
    {
        return ERROR_ELEMENT_NOT_FOUND;
    }

    if (prev == NULL)
    {
        set->elements = current->next;
    }
    else
    {
        prev->next = current->next;
    }
    
    free(current);

    return SUCCESS;
}

int copy_list(
    node_t* source, 
    node_t** dest)
{
    node_t* current = source;
    node_t* new_node = NULL;
    node_t* tail = NULL;
    int status = 0;

    *dest = NULL;

    while (current != NULL)
    {
        status = create_node(&new_node, current->data);
        
        if (status != SUCCESS)
        {
            return status;
        }

        if (*dest == NULL)
        {
            *dest = new_node;
            tail = new_node;
        }
        else
        {
            tail->next = new_node;
            tail = new_node;
        }

        current = current->next;
    }

    return SUCCESS;
}

int free_list(
    node_t* list)
{
    node_t* current = list;
    node_t* temp = NULL;

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        
        free(temp);
    }

    return SUCCESS;
}

int print_subset(
    node_t* subset)
{
    node_t* current = subset;

    printf("{ ");
    
    while (current != NULL)
    {
        printf("%c ", current->data);
        current = current->next;
    }
    
    printf("}\n");

    return SUCCESS;
}

//int generate_powerset_recursive(
//    node_t* elements, 
//    node_t* current_subset, 
//    int index, 
//    int total)
//{
//    node_t* temp = elements;
//    node_t* new_node = NULL;
//    node_t* last = NULL;
//    int i = 0;
//    int status = 0;
//
//    print_subset(current_subset);
//
//    for (i = index; i < total; i++)
//    {
//        temp = elements;
//        
//        while (temp != NULL && i > 0)
//        {
//            temp = temp->next;
//            --i;
//        }
//
//        if (temp == NULL)
//        {
//            break;
//        }
//
//        status = create_node(&new_node, temp->data);
//        
//        if (status != SUCCESS)
//        {
//            return status;
//        }
//
//        if (current_subset == NULL)
//        {
//            status = generate_powerset_recursive(elements, new_node, i + 1, total);
//           
//            free(new_node);
//        }
//        else
//        {
//            last = current_subset;
//           
//            while (last->next != NULL)
//            {
//                last = last->next;
//            }
//           
//            last->next = new_node;
//
//            status = generate_powerset_recursive(elements, current_subset, i + 1, total);
//
//            last->next = NULL;
//            
//            free(new_node);
//        }
//
//        if (status != SUCCESS)
//        {
//            return status;
//        }
//
//        i = index;
//    }
//
//    return SUCCESS;
//}

int count_elements(
    node_t* elements)
{
    int count = 0;
    node_t* current = elements;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    
    return count;
}

int generate_powerset_iterative(
    node_t* elements,
    int n)
{
    long total_subsets = 1L << n;
    long i = 0;
    int j = 0;
    node_t* temp = NULL;

    for (i = 0; i < total_subsets; i++)
    {
        printf("{ ");
        temp = elements;
        j = 0;

        while (temp != NULL)
        {
            if (i & (1L << j))
            {
                printf("%c ", temp->data);
            }
            
            temp = temp->next;
            j++;
        }

        printf("}\n");
    }

    return SUCCESS;
}

int power_set(
    set_list* sets, 
    char set_name)
{
    set_list* set = NULL;
    int status = 0;
    int n = 0;

    status = find_set(sets, set_name, &set);
   
    if (status != SUCCESS)
    {
        return status;
    }

    printf("Power set of %c:\n", set_name);

    n = count_elements(set->elements);

    if (n > 20)
    {
        printf("Warning: set is too large (more than 20 elements).\n");
        
        return SUCCESS;
    }

    status = generate_powerset_iterative(set->elements, n);

    return status;
}

int see_set(
    set_list* sets,
    char set_name)
{
    set_list* set = NULL;
    node_t* current = NULL;
    int status = 0;

    status = find_set(sets, set_name, &set);
   
    if (status != SUCCESS)
    {
        return status;
    }

    printf("%c: { ", set_name);
    
    current = set->elements;
    
    while (current != NULL)
    {
        printf("%c ", current->data);
        current = current->next;
    }
    
    printf("}\n");

    return SUCCESS;
}

int see_all_sets(
    set_list* sets)
{
    set_list* current = sets;

    if (current == NULL)
    {
        printf("No sets available.\n");
        
        return SUCCESS;
    }

    printf("Available sets:\n");
   
    while (current != NULL)
    {
        printf("%c ", current->name);
        current = current->next;
    }
   
    printf("\n");

    return SUCCESS;
}

int union_sets(
    set_list* sets,
    char name_a, 
    char name_b)
{
    set_list* set_a = NULL;
    set_list* set_b = NULL;
    node_t* curr_a = NULL;
    node_t* curr_b = NULL;
    int status = 0;

    status = find_set(sets, name_a, &set_a);
    
    if (status != SUCCESS)
    {
        return status;
    }

    status = find_set(sets, name_b, &set_b);
    
    if (status != SUCCESS)
    {
        return status;
    }

    printf("%c + %c = { ", name_a, name_b);

    curr_a = set_a->elements;
    curr_b = set_b->elements;

    while (curr_a != NULL && curr_b != NULL)
    {
        if (curr_a->data < curr_b->data)
        {
            printf("%c ", curr_a->data);
            curr_a = curr_a->next;
        }
        else if (curr_a->data > curr_b->data)
        {
            printf("%c ", curr_b->data);
            curr_b = curr_b->next;
        }
        else
        {
            printf("%c ", curr_a->data);
            curr_a = curr_a->next;
            curr_b = curr_b->next;
        }
    }

    while (curr_a != NULL)
    {
        printf("%c ", curr_a->data);
        curr_a = curr_a->next;
    }

    while (curr_b != NULL)
    {
        printf("%c ", curr_b->data);
        curr_b = curr_b->next;
    }

    printf("}\n");
   
    return SUCCESS;
}

int intersection_sets(
    set_list* sets,
    char name_a,
    char name_b)
{
    set_list* set_a = NULL;
    set_list* set_b = NULL;
    node_t* curr_a = NULL;
    node_t* curr_b = NULL;
    int status = 0;

    status = find_set(sets, name_a, &set_a);
   
    if (status != SUCCESS)
    {
        return status;
    }

    status = find_set(sets, name_b, &set_b);
   
    if (status != SUCCESS)
    {
        return status;
    }

    printf("%c & %c = { ", name_a, name_b);

    curr_a = set_a->elements;
    curr_b = set_b->elements;

    while (curr_a != NULL && curr_b != NULL)
    {
        if (curr_a->data < curr_b->data)
        {
            curr_a = curr_a->next;
        }
        else if (curr_a->data > curr_b->data)
        {
            curr_b = curr_b->next;
        }
        else
        {
            printf("%c ", curr_a->data);
            curr_a = curr_a->next;
            curr_b = curr_b->next;
        }
    }

    printf("}\n");
   
    return SUCCESS;
}

int difference_sets(
    set_list* sets, 
    char name_a, 
    char name_b)
{
    set_list* set_a = NULL;
    set_list* set_b = NULL;
    node_t* curr_a = NULL;
    node_t* curr_b = NULL;
    int status = 0;

    status = find_set(sets, name_a, &set_a);
   
    if (status != SUCCESS)
    {
        return status;
    }

    status = find_set(sets, name_b, &set_b);
   
    if (status != SUCCESS)
    {
        return status;
    }

    printf("%c - %c = { ", name_a, name_b);

    curr_a = set_a->elements;
    curr_b = set_b->elements;

    while (curr_a != NULL)
    {
        if (curr_b == NULL || curr_a->data < curr_b->data)
        {
            printf("%c ", curr_a->data);
            curr_a = curr_a->next;
        }
        else if (curr_a->data > curr_b->data)
        {
            curr_b = curr_b->next;
        }
        else
        {
            curr_a = curr_a->next;
            curr_b = curr_b->next;
        }
    }

    printf("}\n");
    
    return SUCCESS;
}

int is_subset(
    set_list* sets, 
    char name_a, 
    char name_b)
{
    set_list* set_a = NULL;
    set_list* set_b = NULL;
    node_t* curr_a = NULL;
    node_t* curr_b = NULL;
    int status = 0;

    status = find_set(sets, name_a, &set_a);
    
    if (status != SUCCESS)
    {
        return status;
    }

    status = find_set(sets, name_b, &set_b);
   
    if (status != SUCCESS)
    {
        return status;
    }

    curr_a = set_a->elements;
    curr_b = set_b->elements;

    while (curr_a != NULL && curr_b != NULL)
    {
        if (curr_a->data < curr_b->data)
        {
            printf("%c < %c: false\n", name_a, name_b);
           
            return SUCCESS;
        }
        else if (curr_a->data > curr_b->data)
        {
            curr_b = curr_b->next;
        }
        else
        {
            curr_a = curr_a->next;
            curr_b = curr_b->next;
        }
    }

    if (curr_a != NULL)
    {
        printf("%c < %c: false\n", name_a, name_b);
    }
    else
    {
        printf("%c < %c: true\n", name_a, name_b);
    }

    return SUCCESS;
}

int are_equal(
    set_list* sets,
    char name_a,
    char name_b)
{
    set_list* set_a = NULL;
    set_list* set_b = NULL;
    node_t* curr_a = NULL;
    node_t* curr_b = NULL;
    int status = 0;

    status = find_set(sets, name_a, &set_a);
    
    if (status != SUCCESS)
    {
        return status;
    }

    status = find_set(sets, name_b, &set_b);
   
    if (status != SUCCESS)
    {
        return status;
    }

    curr_a = set_a->elements;
    curr_b = set_b->elements;

    while (curr_a != NULL && curr_b != NULL)
    {
        if (curr_a->data != curr_b->data)
        {
            printf("%c = %c: false\n", name_a, name_b);
            return SUCCESS;
        }
        curr_a = curr_a->next;
        curr_b = curr_b->next;
    }

    if (curr_a != NULL || curr_b != NULL)
    {
        printf("%c = %c: false\n", name_a, name_b);
    }
    else
    {
        printf("%c = %c: true\n", name_a, name_b);
    }

    return SUCCESS;
}

int process_command(
    set_list** sets,
    char* command)
{
    char cmd[10] = { 0 };
    char arg1 = 0;
    char arg2 = 0;
    char op = 0;
    int status = 0;
    int n = 0;

    n = sscanf(command, "%s %c %c", cmd, &arg1, &arg2);

    if (strcmp(cmd, "new") == 0 && n == 2)
    {
        status = add_new_set(sets, arg1);
        
        if (status == SUCCESS)
        {
            printf("Set %c created.\n", arg1);
        }
       
        return status;
    }

    if (strcmp(cmd, "del") == 0 && n == 2)
    {
        status = delete_set(sets, arg1);
       
        if (status == SUCCESS)
        {
            printf("Set %c deleted.\n", arg1);
        }
        
        return status;
    }

    if (strcmp(cmd, "add") == 0 && n == 3)
    {
        status = add_element(*sets, arg1, arg2);
       
        if (status == SUCCESS)
        {
            printf("Element '%c' added to set %c.\n", arg2, arg1);
        }
        
        return status;
    }

    if (strcmp(cmd, "rem") == 0 && n == 3)
    {
        status = remove_element(*sets, arg1, arg2);
        
        if (status == SUCCESS)
        {
            printf("Element '%c' removed from set %c.\n", arg2, arg1);
        }
        
        return status;
    }

    if (strcmp(cmd, "pow") == 0 && n == 2)
    {
        return power_set(*sets, arg1);
    }

    if (strcmp(cmd, "see") == 0)
    {
        if (n == 1)
        {
            return see_all_sets(*sets);
        }
        else if (n == 2)
        {
            return see_set(*sets, arg1);
        }
    }

    n = sscanf(command, " %c %c %c", &arg1, &op, &arg2);
    
    if (n == 3)
    {
        if (op == '+')
        {
            return union_sets(*sets, arg1, arg2);
        }
        if (op == '&')
        {
            return intersection_sets(*sets, arg1, arg2);
        }
        if (op == '-')
        {
            return difference_sets(*sets, arg1, arg2);
        }
        if (op == '<')
        {
            return is_subset(*sets, arg1, arg2);
        }
        if (op == '=')
        {
            return are_equal(*sets, arg1, arg2);
        }
    }

    return ERROR_INVALID_COMMAND;
}

int free_all_sets(
    set_list** sets)
{
    set_list* current = *sets;
    set_list* temp_set = NULL;
    node_t* elem = NULL;
    node_t* temp_node = NULL;

    while (current != NULL)
    {
        elem = current->elements;
       
        while (elem != NULL)
        {
            temp_node = elem;
            elem = elem->next;
            
            free(temp_node);
        }
        temp_set = current;
        current = current->next;
        
        free(temp_set);
    }
    *sets = NULL;
   
    return SUCCESS;
}

//enum status_codes
//{
//    SUCCESS = 0,
//    ERROR_MEMORY,
//    ERROR_INVALID_NAME,
//    ERROR_SET_NOT_FOUND,
//    ERROR_SET_EXISTS,
//    ERROR_INVALID_COMMAND,
//    ERROR_ELEMENT_NOT_FOUND,
//    ERROR_EMPTY_SET
//};

int print_error(
    int status)
{
    //switch (status)
    //{
    //case ERROR_MEMORY:
    //    printf("Error: Memory allocation failed.\n");
    //    break;
    //case ERROR_INVALID_NAME:
    //    printf("Error: Invalid set name. Use A-Z.\n");
    //    break;
    //case ERROR_SET_NOT_FOUND:
    //    printf("Error: Set not found.\n");
    //    break;
    //case ERROR_SET_EXISTS:
    //    printf("Error: Set already exists.\n");
    //    break;
    //case ERROR_INVALID_COMMAND:
    //    printf("Error: Invalid command.\n");
    //    break;
    //case ERROR_ELEMENT_NOT_FOUND:
    //    printf("Error: Element not found.\n");
    //    break;
    //case ERROR_EMPTY_SET:
    //    printf("Error: Set is empty.\n");
    //    break;
    //default:
    //    break;
    //}
    //return SUCCESS;

    char const* errors_arr[8] = { "Error: undefined... 0_o\n", "Error: Memory allocation failed.\n", "Error: Invalid set name. Use A-Z.\n", "Error: Set not found.\n", "Error: Set already exists.\n", "Error: Invalid command.\n", "Error: Element not found.\n", "Error: Set is empty.\n" };

    if (status > 0 && status < 8)
    {
        printf("%s", errors_arr[status]);
    }
    else
    {
        printf("%s", errors_arr[0]);
    }

    return SUCCESS;
}

int main()
{
    set_list* sets = NULL;
    char command[256] = { 0 };
    int status = 0;

    printf("Set Operations Program\n");
    printf("Commands: new, del, add, rem, pow, see, +, &, -, <, =\n");
    printf("Type 'quit' to exit.\n\n");

    while (1)
    {
        printf("> ");
       
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }

        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "quit") == 0)
        {
            break;
        }

        if (strlen(command) == 0)
        {
            continue;
        }

        status = process_command(&sets, command);
        
        if (status != SUCCESS)
        {
            print_error(status);
        }
    }

    free_all_sets(&sets);

    return SUCCESS;
}