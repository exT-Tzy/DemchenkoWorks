#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int fd[2];
    pid_t pid;
    char buf[BUFSIZ];
    char const* message = "message example 0000_________ooooo";
    size_t read_bytes, written_bytes;

    if (pipe(fd) == -1)
    {
        return EXIT_FAILURE;
    }

    pid = fork();

    if (pid == -1)
    {
        return EXIT_FAILURE;
    }
    else if (pid == 0)
    {
        close(fd[1]);

        read_bytes = read(fd[0], buf, BUFSIZ - 1);
        if (read_bytes == -1)
        {
            perror("read");

            exit(EXIT_FAILURE);
        }

        buf[read_bytes] = 0;
        printf("im received: %s\n", buf);

        close(fd[0]);
    }
    else
    {
        close(fd[0]);

        written_bytes = write(fd[1], message, (strlen(message) + 1) * sizeof(char));
        if (written_bytes == -1)
        {
            perror("write");

            exit(EXIT_FAILURE);
        }

        close(fd[1]);

        wait(NULL);
    }

    return 0;
}