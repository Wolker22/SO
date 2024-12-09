#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{

    pid_t child_pid, wpid;
    int status;

    if (argc != 6)
    {
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 2; i++)
    {
        child_pid = fork();
        if (child_pid == -1)
        { /* fork() failed */
            perror("fork");
        }

        if (child_pid == 0)
        { /* This is the child */
            /* Child does some work and then terminates */
            if (i == 0)
            {
                printf("%d: HOLA SOY EL PROCESO %d y voy a ejecutar la calculadora\n", i , getpid());
                execlp(argv[1], argv[1], NULL);
                exit(EXIT_FAILURE);
            }
            if (i == 1)
            {
                printf("%d: HOLA SOY EL PROCESO %d y voy a ejecutar gdit\n", i, getpid());
                execlp("ls", argv[2], argv[3], argv[4], argv[5], NULL);
                exit(EXIT_FAILURE);
            }
            else
            {
                exit(EXIT_FAILURE);
            }
        }
    }

    wpid = waitpid(child_pid, &status, WUNTRACED);
    if (wpid == -1)
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
    else if (WIFCONTINUED(status))
    {
        printf("child continued\n");
    }
    else
    { /* Non-standard case -- may never happen */
        printf("Unexpected status (0x%x)\n", status);
    }
    exit(EXIT_SUCCESS);
}
