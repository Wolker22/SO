#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

struct numeros
{
    int par;
    int impar;
    int i;
};
struct numeros *numeros;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

int funcion(void *i)
{
    struct numeros *numeros = (struct numeros *)i;

    int s, numero;
    int *res = malloc(sizeof(int));
    *res = 0;

    s = pthread_mutex_lock(&mtx);
    if (s != 0)
    {
        printf("mutex_lock error...\n");
        pthread_exit(NULL);
    }

    for (int i = 0; i < 5; i++)
    {
        numero = rand() % 11;
        *res = *res + numero;
    }

    if ((*res) % 2 == 0)
    {
        numeros->par = numeros->par + 1;
    }
    else
    {
        numeros->impar = numeros->impar + 1;
    }

    numeros->i=numeros->i+1;

    s = pthread_mutex_unlock(&mtx);
    if (s != 0)
    {
        printf("mutex_unlock error...\n");
        pthread_exit(NULL);
    }

    pthread_exit((void *)res);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("ERROR\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    int *res;
    pthread_t v[N];
    srand(time(NULL));

    numeros = malloc(sizeof(struct numeros));
    if (numeros == NULL)
    {
        fprintf(stderr, "Error de asignación de memoria para 'numeros'\n");
        exit(EXIT_FAILURE);
    }

    numeros->impar = 0;
    numeros->par = 0;
    numeros->i=0;

    for (int i = 0; i < N; i++)
    {
        if (pthread_create(&v[i], NULL, (void *)funcion, (void *)(numeros)) != 0)
        {
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int j = 0; j < N; j++)
    {
        if (pthread_join(v[j], (void **)&res))
        {
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);
        }
        printf("el valor del hilo %d es %d\n", numeros->i, *res);
    }

    printf("El valor de los impares es; %d\n", numeros->impar);
    printf("El valor de los pares es; %d\n", numeros->par);

    free(res);
    free(numeros);
    return 0;
}