#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
    pid_t h[2];
    pid_t wpid;
    int status;

    for (int i = 0; i < 2; i++)
    {
        h[i] = fork();
        if (h[i] == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (h[i] == 0)
        { // Este es el código de los hijos
            printf("Soy el proceso hijo %d: %d, y mi padre es: %d\n", i + 1, getpid(), getppid());

            if (i == 1)
            {
                printf("Voy a crear un hijo\n");

                pid_t child_pid = fork();
                if (child_pid == -1)
                {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }

                if (child_pid == 0)
                { // Este es el código del nieto
                    printf("Soy el nieto: %d, y mi padre es: %d\n", getpid(), getppid());
                    exit(EXIT_SUCCESS);
                }
                else
                { // Este es el código del hijo
                    wpid = waitpid(child_pid, &status, WUNTRACED);
                    if (wpid == -1)
                    {
                        perror("waitpid nieto");
                        exit(EXIT_FAILURE);
                    }

                    if (WIFEXITED(status))
                    {
                        printf("Nieto terminado, status=%d\n", WEXITSTATUS(status));
                    }
                    else if (WIFSIGNALED(status))
                    {
                        printf("Nieto matado (señal %d)\n", WTERMSIG(status));
                    }
                    else if (WIFSTOPPED(status))
                    {
                        printf("Nieto detenido (señal %d)\n", WSTOPSIG(status));
                    }
                    else if (WIFCONTINUED(status))
                    {
                        printf("Nieto continuado\n");
                    }
                    else
                    {
                        printf("Estado inesperado del nieto (0x%x)\n", status);
                    }
                }
            }
            exit(EXIT_SUCCESS);
        }
    }

    // Esperar a que ambos hijos terminen
    for (int i = 0; i < 2; i++)
    {
        wpid = waitpid(h[i], &status, WUNTRACED);
        if (wpid == -1)
        {
            perror("waitpid hijo");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status))
        {
            printf("Hijo %d terminado, status=%d\n", i + 1, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Hijo %d matado (señal %d)\n", i + 1, WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("Hijo %d detenido (señal %d)\n", i + 1, WSTOPSIG(status));
        }
        else if (WIFCONTINUED(status))
        {
            printf("Hijo %d continuado\n", i + 1);
        }
        else
        {
            printf("Estado inesperado del hijo %d (0x%x)\n", i + 1, status);
        }

        if (wpid == (pid_t)-1 && errno == ECHILD)
        {
            printf("NO HAY MAS HIJOS \n");
        }
    }

    return 0;
}