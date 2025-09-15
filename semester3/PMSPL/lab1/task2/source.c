#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_USERS 100
#define MAX_LOGIN_LEN 6
#define MAX_COMMAND_LEN 256
#define MAX_DATETIME_LEN 30
#define USERS_FILE "users.txt"
#define SANCTIONS_FILE "sanctions.txt"

typedef enum 
{
    STATUS_SUCCESS = 0,
    STATUS_ERROR = 1,
    STATUS_INVALID_INPUT = 2,
    STATUS_USER_NOT_FOUND = 3,
    STATUS_USER_EXISTS = 4,
    STATUS_INVALID_LOGIN = 5,
    STATUS_INVALID_PIN = 6,
    STATUS_WRONG_PIN = 7,
    STATUS_FILE_ERROR = 8,
    STATUS_LOGOUT = 9,
    STATUS_LIMIT_EXCEEDED = 10,
    STATUS_INVALID_DATETIME = 11,
    STATUS_INVALID_FLAG = 12,
    STATUS_CONFIRMATION_FAILED = 13
} StatusCode;

typedef struct 
{
    char login[MAX_LOGIN_LEN + 1];
    int pin;
    int max_commands;
    int commands_used;
} User;

StatusCode is_valid_login(
    char const* login) 
    {
    int len;
    int i;
    
    len = strlen(login);
    if (len == 0 || len > MAX_LOGIN_LEN) 
    {
        return STATUS_INVALID_LOGIN;
    }
    
    for (i = 0; i < len; i++) 
    {
        if (!isalnum(login[i])) 
        {
            return STATUS_INVALID_LOGIN;
        }
    }
   
    return STATUS_SUCCESS;
}

StatusCode is_valid_pin(
    int pin) 
    {
    if (pin >= 0 && pin <= 100000) 
    {
        return STATUS_SUCCESS;
    }
    
    return STATUS_INVALID_PIN;
}

StatusCode load_users(
    User users[],
    int* user_count) 
{
    FILE* file;
    char line[256];
    char login[MAX_LOGIN_LEN + 1];
    int pin;
    int count = 0;
    
    file = fopen(USERS_FILE, "r");
    if (file == NULL) 
    {
        *user_count = 0;
       
        return STATUS_SUCCESS; 
    }
    
    while (fgets(line, sizeof(line), file) && count < MAX_USERS) 
    {
        if (sscanf(line, "%6[^:]:%d", login, &pin) == 2) 
        {
            strcpy(users[count].login, login);
            users[count].pin = pin;
            users[count].max_commands = -1;
            users[count].commands_used = 0;
            
            count++;
        }
    }
    
    fclose(file);
    *user_count = count;
    
    return STATUS_SUCCESS;
}

StatusCode save_users(
    User const users[], 
    int user_count) 
{
    FILE* file;
    int i;
    
    file = fopen(USERS_FILE, "w");
    if (file == NULL) 
    {
        return STATUS_FILE_ERROR;
    }
    
    for (i = 0; i < user_count; i++) 
    {
        fprintf(file, "%s:%d\n", users[i].login, users[i].pin);
    }
    
    fclose(file);
    
    return STATUS_SUCCESS;
}

StatusCode load_sanctions(
    User users[], 
    int user_count) 
{
    FILE* file;
    char line[256];
    char login[MAX_LOGIN_LEN + 1];
    int max_commands;
    int i;
    
    file = fopen(SANCTIONS_FILE, "r");
    if (file == NULL) 
    {
        return STATUS_SUCCESS;
    }
    
    while (fgets(line, sizeof(line), file)) 
    {
        if (sscanf(line, "%6[^:]:%d", login, &max_commands) == 2) 
        {
            for (i = 0; i < user_count; i++) 
            {
                if (strcmp(users[i].login, login) == 0) 
                {
                    users[i].max_commands = max_commands;
                   
                    break;
                }
            }
        }
    }
    
    fclose(file);
   
    return STATUS_SUCCESS;
}

StatusCode save_sanctions(
    User const users[], 
    int user_count) 
{
    FILE* file;
    int i;
    
    file = fopen(SANCTIONS_FILE, "w");
    if (file == NULL) 
    {
        return STATUS_FILE_ERROR;
    }
    
    for (i = 0; i < user_count; i++) 
    {
        if (users[i].max_commands != -1) 
        {
            fprintf(file, "%s:%d\n", users[i].login, users[i].max_commands);
        }
    }
    
    fclose(file);
    
    return STATUS_SUCCESS;
}

StatusCode find_user(
    User const users[], 
    int user_count, 
    char const* login, 
    int* user_index) 
{
    int i;
    
    for (i = 0; i < user_count; i++) 
    {
        if (strcmp(users[i].login, login) == 0) 
        {
            *user_index = i;
           
            return STATUS_SUCCESS;
        }
    }
    
    return STATUS_USER_NOT_FOUND;
}

StatusCode register_user(
    User users[], 
    int* user_count) 
{
    char login[MAX_LOGIN_LEN + 10];
    int pin;
    int user_index;
    StatusCode status;
    
    if (*user_count >= MAX_USERS) 
    {
        printf("Maximum number of users reached!\n");
        
        return STATUS_ERROR;
    }
    
    printf("=== REGISTRATION ===\n");
    printf("Enter login (up to %d characters, alphanumeric only): ", MAX_LOGIN_LEN);
    if (scanf("%s", login) != 1) 
    {
        return STATUS_INVALID_INPUT;
    }
    
    status = is_valid_login(login);
    if (status != STATUS_SUCCESS) 
    {
        printf("Invalid login! Use only letters and digits, length up to %d characters.\n", MAX_LOGIN_LEN);
        
        return status;
    }
    
    status = find_user(users, *user_count, login, &user_index);
    if (status == STATUS_SUCCESS) 
    {
        printf("User with this login already exists!\n");
       
        return STATUS_USER_EXISTS;
    }
    
    printf("Enter PIN (number from 0 to 100000): ");
    if (scanf("%d", &pin) != 1) 
    {
        return STATUS_INVALID_INPUT;
    }
    
    status = is_valid_pin(pin);
    if (status != STATUS_SUCCESS) 
    {
        printf("Invalid PIN! Use number from 0 to 100000.\n");
       
        return status;
    }
    
    strcpy(users[*user_count].login, login);
    users[*user_count].pin = pin;
    users[*user_count].max_commands = -1;
    users[*user_count].commands_used = 0;
    (*user_count)++;
    
    status = save_users(users, *user_count);
    if (status != STATUS_SUCCESS) 
    {
        printf("Error saving user data!\n");
      
        return status;
    }
    
    printf("User successfully registered!\n\n");
   
    return STATUS_SUCCESS;
}

StatusCode login_user(
    const User users[], 
    int user_count, 
    int* current_user) 
{
    char login[MAX_LOGIN_LEN + 10];
    int pin;
    int user_index;
    StatusCode status;
    
    printf("=== LOGIN ===\n");
    printf("Login: ");
    if (scanf("%s", login) != 1) 
    {
        return STATUS_INVALID_INPUT;
    }
    
    printf("PIN: ");
    if (scanf("%d", &pin) != 1)
    {
        return STATUS_INVALID_INPUT;
    }
    
    status = find_user(users, user_count, login, &user_index);
    if (status != STATUS_SUCCESS) 
    {
        printf("User not found!\n\n");
        
        return STATUS_USER_NOT_FOUND;
    }
    
    if (users[user_index].pin != pin) 
    {
        printf("Wrong PIN!\n\n");
        
        return STATUS_WRONG_PIN;
    }
    
    *current_user = user_index;
    printf("Welcome, %s!\n\n", users[user_index].login);
    
    return STATUS_SUCCESS;
}

StatusCode cmd_time() 
{
    time_t now;
    struct tm *local_time;
    
    now = time(NULL);
    local_time = localtime(&now);
    printf("Current time: %02d:%02d:%02d\n", 
           local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
  
    return STATUS_SUCCESS;
}

StatusCode cmd_date() 
{
    time_t now;
    struct tm *local_time;
    
    now = time(NULL);
    local_time = localtime(&now);
    
    printf("Current date: %02d:%02d:%04d\n", 
           local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);
    
    return STATUS_SUCCESS;
}

StatusCode parse_datetime(
    char const* datetime_str, 
    time_t* result) 
{
    struct tm tm_time = {0};
    int day, month, year, hour, minute, second;
    
    if (sscanf(datetime_str, "%d:%d:%d %d:%d:%d", 
               &day, &month, &year, &hour, &minute, &second) != 6) 
    {
        return STATUS_INVALID_DATETIME;
    }
    
    if (day < 1 || day > 31 || month < 1 || month > 12 || 
        year < 1900 || hour < 0 || hour > 23 || 
        minute < 0 || minute > 59 || second < 0 || second > 59) {
        
        return STATUS_INVALID_DATETIME;
    }
    
    tm_time.tm_mday = day;
    tm_time.tm_mon = month - 1;
    tm_time.tm_year = year - 1900;
    tm_time.tm_hour = hour;
    tm_time.tm_min = minute;
    tm_time.tm_sec = second;
    
    *result = mktime(&tm_time);
    if (*result == -1) 
    {
        return STATUS_INVALID_DATETIME;
    }
    
    return STATUS_SUCCESS;
}

StatusCode cmd_howmuch(
    char const* flag, 
    char const* datetime_str) 
{
    time_t target_time;
    time_t now;
    double diff;
    StatusCode status;
    
    status = parse_datetime(datetime_str, &target_time);
    if (status != STATUS_SUCCESS) 
    {
        printf("Invalid date/time format! Use: dd:MM:yyyy hh:mm:ss\n");
        
        return status;
    }
    
    now = time(NULL);
    diff = difftime(now, target_time);
    
    if (diff < 0) 
    {
        printf("Specified time is in the future!\n");
        
        return STATUS_ERROR;
    }
    
    if (strcmp(flag, "-s") == 0) 
    {
        printf("Elapsed seconds: %.0f\n", diff);
    } 
    else if (strcmp(flag, "-m") == 0) 
    {
        printf("Elapsed minutes: %.2f\n", diff / 60.0);
    } 
    else if (strcmp(flag, "-h") == 0)
    {
        printf("Elapsed hours: %.2f\n", diff / 3600.0);
    } 
    else if (strcmp(flag, "-y") == 0) 
    {
        printf("Elapsed years: %.6f\n", diff / (365.25 * 24 * 3600));
    } 
    else 
    {
        printf("Unknown flag! Use: -s, -m, -h, -y\n");
        
        return STATUS_INVALID_FLAG;
    }
    
    return STATUS_SUCCESS;
}

StatusCode cmd_sanctions(
    User users[], 
    int user_count) 
{
    char username[MAX_LOGIN_LEN + 10];
    int max_commands;
    int confirmation;
    int user_index;
    StatusCode status;
    
    printf("Enter username for restrictions: ");
    if (scanf("%s", username) != 1) 
    {
        return STATUS_INVALID_INPUT;
    }
    
    status = find_user(users, user_count, username, &user_index);
    if (status != STATUS_SUCCESS) 
    {
        printf("User not found!\n");
        
        return STATUS_USER_NOT_FOUND;
    }
    
    printf("Enter maximum commands per session: ");
    if (scanf("%d", &max_commands) != 1) 
    {
        return STATUS_INVALID_INPUT;
    }
    
    if (max_commands < 0) 
    {
        printf("Number of commands cannot be negative!\n");
        
        return STATUS_INVALID_INPUT;
    }
    
    printf("Enter 52 for confirmation: ");
    if (scanf("%d", &confirmation) != 1) 
    {
        return STATUS_INVALID_INPUT;
    }
    
    if (confirmation != 52) 
    {
        printf("Wrong confirmation! Restrictions not set.\n");
       
        return STATUS_CONFIRMATION_FAILED;
    }
    
    users[user_index].max_commands = max_commands;
    
    status = save_sanctions(users, user_count);
    if (status != STATUS_SUCCESS) 
    {
        printf("Error saving sanctions!\n");
        
        return status;
    }
    
    printf("Restrictions for user %s set: max %d commands per session.\n", 
           username, max_commands);
    
    return STATUS_SUCCESS;
}

StatusCode check_command_limit(
    User* user) 
{
    if (user->max_commands == -1) 
    {
        return STATUS_SUCCESS;
    }
    
    if (user->commands_used >= user->max_commands) 
    {
        printf("Maximum commands per session reached (%d)!\n", user->max_commands);
        printf("Use 'logout' command to exit.\n");
        
        return STATUS_LIMIT_EXCEEDED;
    }
    
    return STATUS_SUCCESS;
}

StatusCode shell_main(
    User users[], 
    int user_count, 
    int current_user) 
{
    char input[MAX_COMMAND_LEN];
    char command[MAX_COMMAND_LEN];
    char arg1[MAX_COMMAND_LEN];
    char arg2[MAX_COMMAND_LEN];
    char datetime_combined[MAX_DATETIME_LEN];
    StatusCode status;
    
    users[current_user].commands_used = 0;
    
    printf("=== COMMAND LINE SHELL ===\n");
    printf("Available commands:\n");
    printf("  time - current time\n");
    printf("  date - current date\n");
    printf("  howmuch <flag> <dd:MM:yyyy hh:mm:ss> - time since specified moment\n");
    printf("    flags: -s (seconds), -m (minutes), -h (hours), -y (years)\n");
    printf("  sanctions - set user restrictions\n");
    printf("  logout - exit\n\n");
    
    while (1) 
    {
        printf("%s> ", users[current_user].login);
        
        if (fgets(input, sizeof(input), stdin) == NULL) 
        {
            continue;
        }

        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0) 
        {
            continue;
        }

        if (sscanf(input, "%s", command) != 1) 
        {
            continue;
        }
        
        if (strcmp(command, "logout") == 0) 
        {
            printf("Goodbye!\n\n");
            
            return STATUS_LOGOUT;
        }

        status = check_command_limit(&users[current_user]);
        if (status != STATUS_SUCCESS) 
        {
            continue;
        }
        
        users[current_user].commands_used++;
        
        if (strcmp(command, "time") == 0) 
        {
            cmd_time();
        } 
        else if (strcmp(command, "date") == 0) 
        {
            cmd_date();
        } 
        else if (strcmp(command, "howmuch") == 0) 
        {
            if (sscanf(input, "%s %s %s %s", command, arg1, arg2, datetime_combined) == 4) 
            {
                strcat(arg2, " ");
                strcat(arg2, datetime_combined);
                cmd_howmuch(arg1, arg2);
            } else 
            {
                printf("Usage: howmuch <flag> <dd:MM:yyyy hh:mm:ss>\n");
            }
        } 
        else if (strcmp(command, "sanctions") == 0) 
        {
            cmd_sanctions(users, user_count);
        } 
        else 
        {
            printf("Unknown command: %s\n", command);
        }
    }
    
    return STATUS_SUCCESS;
}

StatusCode main_menu(
    User users[], 
    int* user_count, 
    int* current_user) 
{
    int choice;
    StatusCode status;
    
    while (1) 
    {
        if (*current_user == -1) 
        {
            printf("1. Login\n");
            printf("2. Registration\n");
            printf("3. Exit\n");
            printf("Choose action (1-3): ");
            
            if (scanf("%d", &choice) != 1) 
            {
                printf("Invalid input!\n\n");
                while (getchar() != '\n');
                
                continue;
            }

            while (getchar() != '\n');
            
            switch (choice) 
            {
                case 1:
                    
                    status = login_user(users, *user_count, current_user);
                    if (status == STATUS_SUCCESS) 
                    {
                        status = shell_main(users, *user_count, *current_user);
                        if (status == STATUS_LOGOUT) 
                        {
                            *current_user = -1;
                        }
                    }
                   
                    break;
                
                case 2:
                    
                    register_user(users, user_count);
                    
                    break;
                
                case 3:
                    
                    printf("Goodbye!\n");
                    
                    return STATUS_SUCCESS;
                
                default:
                    printf("Invalid choice!\n\n");
            }
        }
    }
}

int main() 
{
    User users[MAX_USERS];
    int user_count = 0;
    int current_user = -1;
    StatusCode status;
    
    printf("=== WELCOME TO COMMAND LINE SHELL ===\n\n");
    
    status = load_users(users, &user_count);
    if (status != STATUS_SUCCESS) 
    {
        printf("Error loading user data!\n");
        
        return 1;
    }
    
    status = load_sanctions(users, user_count);
    if (status != STATUS_SUCCESS) 
    {
        printf("Error loading sanctions data!\n");
        
        return 1;
    }
    
    status = main_menu(users, &user_count, &current_user);
    
    return (status == STATUS_SUCCESS) 
    ? 0 
    : 1;
}