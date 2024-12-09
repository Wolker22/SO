#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int status;
    pid_t wpid, pid;

    if (argc != 2)
    {
        perror("ERROR\n");
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);

    for (int i = 0; i < n; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("ERROR\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            printf("%d: soy el hijo: %d y el pid de mi padre es: %d\n", i, getpid(), getppid());
            exit(EXIT_SUCCESS);
        }
    }
    while (wpid = wait(&status)>0)
    {
        if (pid == -1)
        {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status))
        {
            printf("child exited, status=%d\n", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("child killed (signal %d)\n", WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("child stopped (signal %d)\n", WSTOPSIG(status));
        }
    }
}