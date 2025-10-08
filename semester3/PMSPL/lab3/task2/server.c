#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_MESSAGE_LENGTH 100
#define FIFO_NAME "fifo"
#define TARGET 5

int main()
{
    int fd;
    size_t counts[99] = { 0 };
    char buf[BUFSIZ];
    size_t read_bytes, message_length;

    printf("Server is starting...\n");

    if (mkfifo(FIFO_NAME, 0600) < 0)
    {
        perror("mkfifo");
        
        return EXIT_FAILURE;
    }

    printf("SERVER: FIFO created.\n");
    printf("Server started.\n");
    printf("SERVER: waiting for client...\n");

    fd = open(FIFO_NAME, O_RDONLY);
    if (fd < 0)
    {
        perror("open FIFO on server");

        unlink(FIFO_NAME);
        exit(EXIT_FAILURE);
    }

    printf("SERVER: FIFO opened.\n");

    while (1)
    {
        read_bytes = read(fd, buf, MAX_MESSAGE_LENGTH + 1);
        buf[read_bytes] = 0;

        message_length = strlen(buf);

        if (message_length < MAX_MESSAGE_LENGTH)
        {
            counts[message_length - 1] += 1;
            
            printf("SERVER: received \"%s\" (length = %zu)\n", buf, message_length);

            if (counts[message_length - 1] >= TARGET)
            {
                printf("SERVER: count of messages with length %zu reached target.\n", message_length);
                printf("Server shutdown.\n");

                break;
            }
        }
    }


    close(fd);
    unlink(FIFO_NAME);
    return EXIT_SUCCESS;
}