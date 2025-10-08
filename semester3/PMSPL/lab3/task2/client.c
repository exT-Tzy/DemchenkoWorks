#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_MESSAGE_LENGTH 100
#define FIFO_NAME "fifo"
#define TARGET 5
#define CHARSET_LENGTH 62

void generate_random_string(
    char * buf,
    size_t max_length)
{
    size_t rand_length = 1 + (rand() % (MAX_MESSAGE_LENGTH - 1));
    char const charset[CHARSET_LENGTH] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    int i;

    for (i = 0; i < rand_length; ++i)
    {
        buf[i] = charset[rand() % CHARSET_LENGTH];
    }
    
    buf[rand_length] = 0;
}
    
int main()
{
    int fd;
    char buf[BUFSIZ];
    size_t written_bytes;

    fd = open(FIFO_NAME, O_WRONLY);
    if (fd < 0)
    {
        perror("open FIFO on client");

        return EXIT_FAILURE;
    }

    while (1)
    {
        generate_random_string(buf, MAX_MESSAGE_LENGTH);

        printf("CLIENT: sending \"%s\".\n", buf);

        written_bytes = write(fd, buf, strlen(buf) * sizeof(char));
        if (written_bytes < 0)
        {
            if (errno == EPIPE)
            {
                printf("CLIENT: server closed FIFO.\n");
                break;
            }
            else
            {
                perror("write");
                break;
            }
        }
    }
        
    close(fd);
    return EXIT_SUCCESS;
}