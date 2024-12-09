#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    pid_t child_pid, wpid;
    int status;
    for (int i = 0; i < n; i++)
    {
        child_pid = fork();
        if (child_pid == -1)
        { /* fork() failed */
            perror("fork");
            printf("Error al crear el proceso");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0)
        { /* This is the child */

            printf("HOLA SOY EL HIJO: %d Y MI PADRE ES: %d\n", getpid(), getppid());
        }
        else
        {
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
            break;
        }
    }
    exit(EXIT_SUCCESS);
}