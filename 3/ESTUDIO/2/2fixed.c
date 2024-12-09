#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct mystruct
{
    int par;
    int impar;
    int i;
    pthread_mutex_t *mutex;
};

void *mifuncion(void *p)
{
    struct mystruct *jar = (struct mystruct *)p;
    for (int i = 0; i < 5; i++)
    {
        int n = rand() % (11) + 10;
        printf("%d\n", n);
        if (pthread_mutex_lock(jar->mutex) != 0)
        {
            perror("Error al bloquear hilo\n");
            exit(EXIT_FAILURE);
        }
        if (n % 2 == 0)
        {
            jar->par = jar->par + 1;
        }
        else
        {
            jar->impar = jar->impar + 1;
        }
        if (pthread_mutex_unlock(jar->mutex) != 0)
        {
            perror("Error al desbloquear hilo\n");
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("Error al iniciar el programa\n");
        exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    srand(time(NULL));

    struct mystruct *p = malloc(sizeof(struct mystruct) * n);
    pthread_t h[n];
    pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

    for (int i = 0; i < n; i++)
    {
        p[i].i = i;
        p[i].impar = 0;
        p[i].par = 0;
        p[i].mutex = &m;
        if (pthread_create(&h[i], NULL, mifuncion, (void *)&p[i]) != 0)
        {
            perror("Error al iniciar el crear el hilo\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (pthread_join(h[i], NULL) != 0)
        {
            perror("Error al recoger el hilo\n");
            exit(EXIT_FAILURE);
        }
        printf("PAR: %d, IMPAR: %d, Del hilo numero: %d\n", p[i].par, p[i].impar, p[i].i);
    }

    free(p); // Liberar la memoria asignada dinámicamente
    return 0;
}