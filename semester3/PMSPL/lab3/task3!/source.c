#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define TARGET 50

int gcd(
    int a, 
    int b) 
{
  while (b != 0) 
  {
    int t = b;
    b = a % b;
    a = t;
  }
  
  return a;
}

int is_prime(
    int n)
{
    int i;
    
    if (n < 2) return 0;
   
    for (i = 2; i * i <= n; ++i)
    {
        if (n % i == 0) return 0;
    }
    
    return 1;
}

void child_process(
    int read_fd, 
    int write_fd)
{
    ssize_t read_bytes;
    int n, res_count, i;

    while(1)
    {
        read_bytes = read(read_fd, &n, sizeof(n));
        if (read_bytes == -1)
        {
            perror("read");
            
            break;
        }
        if (read_bytes == 0)
        { 
            break;
        }

        res_count = 0;
        for (i = 1; i < n; ++i)
        {
            if (gcd(i, n) == 1)
            {
                res_count++;
            }
        }

        if (write(write_fd, &res_count, sizeof(res_count)) == -1)
        {
            perror("write");
            
            break;
        }
    }
}

void parent_process(
    int write_fd, 
    int read_fd)
{
    ssize_t written_bytes, read_bytes;
    size_t count = 0;
    int n, res_count;

    srand(time(NULL));

    while (count < TARGET)
    {
        n = rand() % 1000 + 1;
        if (write(write_fd, &n, sizeof(n)) == -1)
        {
            perror("write");
            
            break;
        }

        if (read(read_fd, &res_count, sizeof(res_count)) == - 1)
        {
            perror("read");
            
            break;
        }

        if (res_count == n - 1)
        {
            ++count;
            
            printf("PARENT: received matching system for n = %d.\n", n);
        }
    }
}

int main()
{
    int pipe1[2], pipe2[2];
    pid_t pid;

    if (pipe(pipe1) != 0 || pipe(pipe2) != 0)
    {
        return EXIT_FAILURE;
    }

    pid = fork();
    if (pid < 0)
    {
        return EXIT_FAILURE;
    }
    if (pid == 0)
    {
        close(pipe1[1]);
        close(pipe2[0]);

        child_process(pipe1[0], pipe2[1]);

        close(pipe1[0]);
        close(pipe2[1]);
        
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(pipe1[0]);
        close(pipe2[1]);

        parent_process(pipe1[1], pipe2[0]);

        close(pipe1[1]);
        close(pipe2[0]);

        wait(NULL);

        printf("PARENT: completed successfully.\n");
    }

    return EXIT_SUCCESS;
}