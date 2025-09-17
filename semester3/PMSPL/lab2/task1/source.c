#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void print_process_attributes()
{
    printf("Process attributes:\n");
    printf("    PID: %d\n", getpid());
    printf("    PPID: %d\n", getppid());
    printf("    PGID: %d\n", getpgrp());
    printf("    RUID: %d\n", getuid());
    printf("    EUID: %d\n", geteuid());
    printf("    RGID: %d\n", getgid());
    printf("    EGID: %d\n", getegid());
}

int main()
{
    print_process_attributes();

    return 0;
}